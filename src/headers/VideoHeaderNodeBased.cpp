 /*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoHeaderNodeBased.h"

namespace ofxPm{

//------------------------------------------------------
    VideoHeaderNodeBased::VideoHeaderNodeBased(): ofxOceanodeNodeModel("Video Header")
    {
        fps         = 11.0;
        oneFrameMs	= 1000.0 / fps;
        inMs        = 0;
        outMs       = 0;
        lengthMs		= inMs - outMs;
//        //buffer 		= NULL;
        lengthMs = 0;
        offsetFrames = 0.0;
        width = -11;
        height = -11;
        
        autoBPM = false;
        oldBufferSize = 0;
        
        setupNodeBased();
    }

    void VideoHeaderNodeBased::setup(ofxPm::VideoBufferNodeBased* _buff)
    {
        fps         = 11.0;
        oneFrameMs	= 1000.0 / fps;
        inMs        = 0;
        outMs       = 0;
        lengthMs		= inMs - outMs;
        //        //buffer 		= NULL;
        lengthMs = 0;
        offsetFrames = 0.0;
        width = -11;
        height = -11;
        

        paramVideoBufferInput = _buff;
//        parameters->get("Buffer Input").cast<ofxPm::VideoBufferNodeBased*>() = _buff;
    }

    //------------------------------------------------------
    void VideoHeaderNodeBased::setupNodeBased()
    {
        color = ofColor::orange;

//        //newFrameEvent.init("Playmodes.VideoHeader.newFrame");
//        this->buffer= &_buffer;
//        fps         = _buffer.getFps();
//        this->buffer->clear();
        oneFrameMs	= 1000.0 / fps;
//        totalBufferSizeInMs = _buffer.getSizeInFrames() * oneFrameMs;
        inMs        = totalBufferSizeInMs;
        outMs       = 0;
        lengthMs	= totalBufferSizeInMs;
        offsetFrames = 0.0;
        
        
        parameters->add(paramVideoBufferInput.set("Buffer Input", nullptr));
        parameters->add(paramManualOrBPM.set("Timming Man/BPM", false));
        ofParameter<char> pc("Manual",' ');
        parameters->add(pc);
        parameters->add(paramDelayMs.set("Delay Ms",0.0,0.0,4000));
        ofParameter<char> pc2("BPM",' ');
        parameters->add(pc2);
        parameters->add(paramBeatDiv.set("Beats Div",1,1,32));
        parameters->add(paramBeatMult.set("Beats Mult",1,1,32));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        
        
        listener = paramVideoBufferInput.newListener(this, &VideoHeaderNodeBased::changedVideoBuffer);
    }

    //------------------------------------------------------
    VideoHeaderNodeBased::~VideoHeaderNodeBased(){
    }

    //------------------------------------------------------
    VideoFrame VideoHeaderNodeBased::getNextVideoFrame()
        {
            
        // frame to be returned;
        VideoFrame frame;
 
            // check that the buffer is not NULL or size = 0
        if(paramVideoBufferInput.get()!=NULL)
        {
            if(paramVideoBufferInput.get()->getSizeInFrames()>0)
            {
                paramVideoBufferInput.get()->lock();
                {
                    // get the next frame timeStamp based on current behaviour
                    currentFrameTs = getNextFrameTimestamp();
                    // fetch closest video frame from buffer
                    frame = paramVideoBufferInput.get()->getVideoFrame(currentFrameTs);
                    // get the index of the fetched frame
                    currentFrameIndex = frame.getBufferIndex();
                }
                paramVideoBufferInput.get()->unlock();
            }
        }
        return frame;
    }

    //------------------------------------------------------
    Timestamp   VideoHeaderNodeBased::getNextFrameTimestamp()
    {
        // to be returned
        Timestamp ts;
        
        // when buffer is not stopped we need to update the nowTS which is used to update TS of delay,inTS,outTS ...
        bool isStopped = paramVideoBufferInput.get()->isStopped();
        int  bufferSizeInFrames = paramVideoBufferInput.get()->getSizeInFrames();
        if(!isStopped && (bufferSizeInFrames>0))
        {
            nowTS = paramVideoBufferInput.get()->getFirstFrameTimestamp();
        }

        //update in and out TS based on "nowTS"
        inTS.update();
        outTS.update();
        
        inTS = nowTS - TimeDiff(inMs*1000);
        outTS = nowTS - TimeDiff(outMs*1000);
        
        // calculate the ts of the needed frame
        if(!paramManualOrBPM)
        {
            // delay expressed manually in Ms
            ts = nowTS - TimeDiff(paramDelayMs*1000);
        }
        else
        {
            // delay expressed by BPM,div and mult;
            float BPMfactor;
            if(paramBeatDiv!=0)
            {
                BPMfactor = (float(paramBeatMult)/float(paramBeatDiv));
            }
            else  BPMfactor = 1.0;
            float oneBeatMs = (60.0/myBPM)*1000;
            float oneBeatMsBPM = oneBeatMs / BPMfactor;

            ts = nowTS - TimeDiff(oneBeatMsBPM*1000);
            //cout << "VideoHeader : BPM " << myBPM << " _ OneBeatMs = " << oneBeatMs << " _ BPM Factor = " << BPMfactor << "oneBeatBPM = " << oneBeatMsBPM<<  endl;
            
            
        }

        return ts;
    }

    //------------------------------------------------------
    float VideoHeaderNodeBased::getFps(){
        return fps;
    }
    //------------------------------------------------------
    void VideoHeaderNodeBased::setFps(float fps){
        this->fps=fps;
    }
    //------------------------------------------------------
    /*VideoFrame VideoHeader::getVideoFrame(int index)
    {
        buffer->lock();
            int indexFrame = CLAMP(index,0,buffer->size()-1);
            VideoFrame frame = buffer->getVideoFrame(indexFrame);
        buffer->unlock();
        return frame;
    }
     */
    //------------------------------------------------------
    VideoBufferNodeBased* VideoHeaderNodeBased::getBuffer()
    {
        return paramVideoBufferInput.get();
    }

    //------------------------------------------------------
    void VideoHeaderNodeBased::setDelayMs(double _delayMs)
    {
        paramDelayMs = _delayMs;
    }


            
    //------------------------------------------------------
    string VideoHeaderNodeBased::getInfo()
    {
        return "";
    }
        
    //------------------------------------------------------
    void	VideoHeaderNodeBased::setOffsetInFrames(int _o)
    {
        offsetFrames = _o;
    }

    //------------------------------------------------------
    void VideoHeaderNodeBased::setInMs(double _in)
    {
        this->inMs = _in;
    }

    //------------------------------------------------------
    void VideoHeaderNodeBased::setOutMs(double _out)
    {
        this->outMs = _out;
    }

    //-----------------------------------------
    void VideoHeaderNodeBased::changedVideoBuffer(ofxPm::VideoBufferNodeBased* &_videoBuffer)
    {
        ofxPm::VideoFrame vf;
        if(_videoBuffer!=NULL)
        {
            vf = getNextVideoFrame();
            
            //
            int bs = paramVideoBufferInput.get()->getBufferSize();
            float fp = paramVideoBufferInput.get()->getFps();
            float oneFrame = 1.0 / fp;
            
            if(paramVideoBufferInput.get()->getBufferSize()!=oldBufferSize)
            {
                paramDelayMs.setMax(oneFrame * bs * 1000);
                paramDelayMs = ofClamp(paramDelayMs,paramDelayMs.getMin(),paramDelayMs.getMax());
                string name = paramDelayMs.getName();
                ofNotifyEvent(parameterChangedMinMax, name);
                oldBufferSize = paramVideoBufferInput.get()->getBufferSize();
            }

        }
        paramFrameOut = vf;

    }
    
}

