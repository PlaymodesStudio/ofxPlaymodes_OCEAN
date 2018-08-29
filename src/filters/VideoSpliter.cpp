#include "VideoSpliter.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    VideoSpliter::VideoSpliter():ofxOceanodeNodeModel("Spliter x2")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    VideoSpliter::~VideoSpliter()
    {
    }

    //--------------------------------------------------------------
    void VideoSpliter::setupNodeBased()
    {
        color = ofColor::yellow;
        fps = -1;
        
        parameters->add(paramFrameIn.set("Frame Input 1", VideoFrame()));
        parameters->add(paramFrameOut.set("Frame Output 1", VideoFrame()));
        parameters->add(paramFrameOut2.set("Frame Output 2", VideoFrame()));

        paramFrameIn.addListener(this, &VideoSpliter::newVideoFrame);

    }
    //------------------------------------------------------------
    void VideoSpliter::update(ofEventArgs &e)
    {
    }
    //--------------------------------------------------------------
    void VideoSpliter::newVideoFrame(VideoFrame & _frame)
    {
        if((!_frame.isNull())&&(!_frame.isNullPtr()))
        {
            paramFrameOut = _frame;
            paramFrameOut2 = _frame;
        }
    }

}
