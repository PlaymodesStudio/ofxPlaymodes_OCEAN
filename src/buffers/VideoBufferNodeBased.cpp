
#include "VideoBufferNodeBased.h"

namespace ofxPm
{

    //------------------------------------------------------
    VideoBufferNodeBased::VideoBufferNodeBased(): ofxOceanodeNodeModel("Video Buffer")
    {
        setupNodeBased(480,false);
    }


    //------------------------------------------------------
    void VideoBufferNodeBased::setupNodeBased(int size, bool allocateOnSetup)
    {
        maxSize = size;
        lastFrameTimeValEpochMs = 0;
        lastFrameTimeValEpochMsAtStop = 0;
        
        VideoSource::width = -1;
        VideoSource::height = -1;
        
        if(allocateOnSetup && VideoSource::width!=-1)
        {
            printf("VideoBufferNodeBased:: allocating on setup %d %d : ",VideoSource::getWidth(),VideoSource::getHeight());
            for(int i=0;i<size;i++)
            {
                VideoFrame videoFrame = VideoFrame();
                newVideoFrame(videoFrame);
                printf("%d-",i);
            }
            printf("//\n");
        }
        else cout << "VideoBufferNodeBased:: Buffer was not allocated on setup." << endl;
        
        microsOneSec=-1;
        
        // parametersGroup
        parameters->add(paramFrameIn.set("Frame Input", VideoFrame()));
        parameters->add(paramIsRecording.set("Is recording",false));
        parameters->add(paramFPS.set("FPS",60,0,60));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        parameters->add(paramVideoBufferOut.set("Buffer Output",nullptr));
        
        paramIsRecording.addListener(this,&VideoBufferNodeBased::changedIsRecording);
        // do this the last to avoid sending nullptr frame
//        resume();
    }


    //-------------------------------------------------------------------
    VideoBufferNodeBased::~VideoBufferNodeBased()
    {

    }

    //-------------------------------------------------------------------
    void VideoBufferNodeBased::newVideoFrame(VideoFrame & frame)
    {
        if(!frame.isNull())
        {
            int w = frame.getWidth();
            int h = frame.getHeight();
            
            VideoSource::setWidth(w);
            VideoSource::setHeight(h);
            
            if(!isStopped())
            {
                int64_t time = frame.getTimestamp().epochMicroseconds();
                if(microsOneSec==-1) microsOneSec=time;
                framesOneSec++;
                int64_t diff = time-microsOneSec;
                if(diff>=1000000){
                    //realFps = double(framesOneSec*1000000.)/double(diff);
                    framesOneSec = 0;
                    microsOneSec = time-(diff-1000000);
                }
                
                //    if(size()==0)initTime=frame.getTimestamp();
                lastFrameTimeValEpochMs = lastFrameTimeValEpochMsAtStop + (frame.getTimestamp().epochMicroseconds() - startRecordingTs.epochMicroseconds());
                //cout << "Buff::NewVideoFrame:: with TS = " << tdiff << " Which comes from frameTS : " << frame.getTimestamp().raw() << " - initTime " << initTime.raw() << endl;
                
                //cout << "Frame : " << frame.getTimestamp().epochMicroseconds() << " __ startRec : " << startRecordingTs.epochMicroseconds() << " MINUS in EpochMS= " << frame.getTimestamp().epochMicroseconds() - startRecordingTs.epochMicroseconds() << endl;
                //cout << ">> >> LAST FRAME ADDED A TS EPOCHMS = " << lastFrameTimeValEpochMs << " IN SECs :: "  << lastFrameTimeValEpochMs/1000000.0 << endl;
                
                frame.setTimestamp(lastFrameTimeValEpochMs);
                //timeMutex.lock();
                frames.push_back(frame);
                //cout << "Buffer : newVideoFrame with TS : " << frame.getTimestamp().raw() << endl;

                
//                Timestamp tis = frame.getTimestamp() - startRecordingTs.epochMicroseconds() ;
//                cout << "Buffer : newVideoFrame with TS : " << frame.getTimestamp().epochMicroseconds() << " startRecTs : " << startRecordingTs.epochMicroseconds() << endl;
//                cout << " | Difference From start of Rec = " << tis.epochMicroseconds()  << endl;
//                cout << " ......... " << endl;
                
                
                while(getSizeInFrames()>maxSize){
                    frames.erase(frames.begin());
                }
                //timeMutex.unlock();
                paramFrameOut = frame;
            }

        }
        paramVideoBufferOut = this;
        //parameters->get("Buffer Output").cast<ofxPm::VideoBufferNodeBased*>() = (ofxPm::VideoBufferNodeBased*)this;
   
    }
    //-------------------------------------------------------------------
    Timestamp VideoBufferNodeBased::getLastTimestamp()
    {
        if(getSizeInFrames()>0)
            return frames.back().getTimestamp();
        else
            return Timestamp();
    }

//    //-------------------------------------------------------------------
//    TimeDiff VideoBufferNodeBased::getTotalTime()
//    {
//        return -1;
    
//        //getLastTimestamp()-getInitTime(); as we've moved to a relative timestamp of frames in buffer
//        //maybe it makes non sense to getTotalTime() ?
//
//    }

//    //-------------------------------------------------------------------
//    Timestamp VideoBufferNodeBased::getInitTime(){
//        return initTime;
//    }

    //----------------------------------------------
    unsigned int VideoBufferNodeBased::getMaxSize(){
        return maxSize;
    }
    
    //----------------------------------------------
    float VideoBufferNodeBased::getFps(){
        return paramFPS;
    }
    
    //----------------------------------------------
    VideoFrame VideoBufferNodeBased::getVideoFrame(Timestamp ts)
    {
        VideoFrame frame;
        int closestPosition=0;
        if(frames.size()>0)
        {
            TimeDiff tdiff = 1000000000000000;
            for(int i=0;i<getSizeInFrames();i++)
            {
                TimeDiff tdiff2 = abs(ts.epochMicroseconds() - frames[i].getTimestamp().epochMicroseconds());
                //cout << "Buffer:GetVFr:: Frame : "<< i << " has a TS of : " << frames[i].getTimestamp().raw()  <<  " and we look for " << ts.raw() << " . The diff is = " << tdiff2 << endl;
                if(tdiff2<tdiff)
                {
                    //cout << "!! Found a closest position !! : "<< i << endl;
                    closestPosition=i;
                    tdiff=tdiff2;
                }
                
            }
            //cout<<"Buffer : Getting frame at closest TS : " << ts.raw()<< " :: Closest Position :: " << closestPosition<<endl;
            if(closestPosition>frames.size())
            {
                closestPosition=frames.size();
            }
            
            frame = frames[closestPosition];
        }
        // ??? is this a good way to go ?
        // i've added a "index position" to a videoFrame ... this allows us to draw header based on pos, not TS?
        frame.setBufferIndex(closestPosition);
        return frame;
    }
    
    //----------------------------------------------
    VideoFrame VideoBufferNodeBased::getVideoFrame(TimeDiff time)
    {
        VideoFrame frame;
        if(getSizeInFrames()>0)
        {
            int frameback = CLAMP((int)((float)time/1000000.0*(float)getFps()),1,int(getSizeInFrames()));
            int currentPos = CLAMP(getSizeInFrames()-frameback,0,getSizeInFrames()-1);
            frame = frames[currentPos];
        }
        
        return frame;
    }
    
    //----------------------------------------------

    VideoFrame VideoBufferNodeBased::getVideoFrame(int position){
        if(getSizeInFrames()){
            position = CLAMP(position,0,int(getSizeInFrames())-1);
            return frames[position];
        }else{
            return VideoFrame();
        }
    }
    
    //----------------------------------------------
    VideoFrame VideoBufferNodeBased::getVideoFrame(float pct)
    {
        return getVideoFrame(getMaxSize()*pct);
    }

    //----------------------------------------------
    VideoFrame VideoBufferNodeBased::getNextVideoFrame()
    {
        return getVideoFrame((int)getSizeInFrames()-1);
    }

    //----------------------------------------------
    unsigned int VideoBufferNodeBased::getSizeInFrames()
    {
        int res = 0;
        if(frames.size()>0)
        {
            if(!frames.empty()) res=frames.size();
        }
        return res;
    }


    //----------------------------------------------
    void VideoBufferNodeBased::stop()
    {
        stopped = true;
        
        stopRecordingTs.update();
        cout << "[] [STOP] [] Buffer STOP recording at Ts Epoch MicroS : " << stopRecordingTs.epochMicroseconds() << endl;
        Timestamp di = stopRecordingTs - startRecordingTs;
        cout << "Difference between start and stop in Epoch MicroS = " << di.epochMicroseconds() << endl;
        
        // when STOP we get the last frame timeValEpocMs(the timestamps of the last frame captured before stop)
        // to be able to keep the timestamping relative to recording time instead of captur absolut time
        lastFrameTimeValEpochMsAtStop = lastFrameTimeValEpochMs;
    }
    
    //----------------------------------------------
    void VideoBufferNodeBased::resume()
    {
        paramFrameIn.addListener(this, &VideoBufferNodeBased::newVideoFrame);
        
        stopped = false;
        
        startRecordingTs.update();

        cout << "[] [RECORDING INTO BUFFER] []" << endl;
        cout << "Buffer started recording at Ts Epoch MicroS : " << startRecordingTs.epochMicroseconds() << endl;
        
    }
    
    //----------------------------------------------
    bool VideoBufferNodeBased::isStopped()
    {
        
        return stopped;
    }
    
    //----------------------------------------------
    void VideoBufferNodeBased::clear(){
        while(!frames.empty()){
            frames.erase(frames.begin());
        }
    }
    
    //----------------------------------------------
    void VideoBufferNodeBased::changedIsRecording(bool& _b)
    {
        if(_b)
        {
            cout << "Buffer : START REC !!" << endl;
            resume();
        }
        else
        {
            cout << "Buffer : STOPING REC !!" << endl;
            stop();
        }
    }
}


