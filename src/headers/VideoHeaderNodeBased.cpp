 /*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoHeaderNodeBased.h"
#include "ofxOceanodeContainer.h"

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
        

        //paramVideoBufferInput = _buff;
        parameters->get("Buffer Input").cast<ofxPm::VideoBufferNodeBased*>() = _buff;
    }

    //------------------------------------------------------
    void VideoHeaderNodeBased::setupNodeBased()
    {
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
        parameters->add(paramDelayMs.set("Delay Ms",0.0,0.0,3000.0));
        parameters->add(paramFrameOut.set("Frame Output",VideoFrame::VideoFrame()));
        
        
        paramVideoBufferInput.addListener(this, &VideoHeaderNodeBased::changedVideoBuffer);
        
        
//        VideoSource::width = _buffer.getWidth();
//        VideoSource::height = _buffer.getHeight();
//        
//        printf("VideoHeader::setup %d %d @ FPS %f\n",_buffer.getWidth(),_buffer.getHeight(),fps);
    }

    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* VideoHeaderNodeBased::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoBufferNodeBased*>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoBufferNodeBased*>(), sink.cast<ofxPm::VideoBufferNodeBased*>());
        }
    }

    //------------------------------------------------------
    VideoHeaderNodeBased::~VideoHeaderNodeBased(){
    }

    //------------------------------------------------------
    VideoFrame VideoHeaderNodeBased::getNextVideoFrame()
        {
            
        // frame to be returned;
        VideoFrame frame;
        
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
        return frame;
    }

    //------------------------------------------------------
    Timestamp   VideoHeaderNodeBased::getNextFrameTimestamp()
    {
        // to be returned
        Timestamp ts;
        
        // when buffer is not stopped we need to update the nowTS which is used to update TS of delay,inTS,outTS ...
        if(!paramVideoBufferInput.get()->isStopped() && (paramVideoBufferInput.get()->getSizeInFrames()>0))
        {
            nowTS = paramVideoBufferInput.get()->getFirstFrameTimestamp();
        }

        //update in and out TS based on "nowTS"
        inTS.update();
        outTS.update();
        
        inTS = nowTS - TimeDiff(inMs*1000);
        outTS = nowTS - TimeDiff(outMs*1000);
        
        // calculate the ts of the needed frame
        ts = nowTS - TimeDiff(paramDelayMs*1000);

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
            //cout << "VideoBufferNodeBased::setting Video Buffer and sending new frame to header !!"  << endl;

            ofxPm::VideoFrame vf;
            if(_videoBuffer!=NULL)
            {
                vf = getNextVideoFrame();
            }
            parameters->get("Frame Output").cast<ofxPm::VideoFrame>() = vf;
    }

    
}

