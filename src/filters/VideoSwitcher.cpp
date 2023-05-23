
#include "VideoSwitcher.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    VideoSwitcher::VideoSwitcher():ofxOceanodeNodeModel("Switcher")
    {
        setupNodeBased();
    }
    
    //--------------------------------------------------------------
    VideoSwitcher::~VideoSwitcher()
    {
    }
    
    //--------------------------------------------------------------
    void VideoSwitcher::setupNodeBased()
    {
        color = ofColor::yellow;
        fps = -1;
        
        addParameter(paramFrameIn.set("Frame Input 1", VideoFrame()));
        addParameter(paramFrameIn2.set("Frame Input 2", VideoFrame()));
        addParameter(paramSwitchTo.set("Switch",0,0,1));
        addParameter(paramFrameOut.set("Frame Output", VideoFrame()));
        
        listeners.push(paramFrameIn.newListener(this, &VideoSwitcher::newVideoFrame));
        listeners.push(paramFrameIn2.newListener(this, &VideoSwitcher::newVideoFrame2));

    }
    //------------------------------------------------------------
    void VideoSwitcher::update(ofEventArgs &e)
    {
        switch(paramSwitchTo.get())
        {
            case 0 :
                paramFrameOut = paramFrameIn;
                break;
            case 1 :
                paramFrameOut = paramFrameIn2;
                break;
        }
    }
    //--------------------------------------------------------------
    void VideoSwitcher::newVideoFrame(VideoFrame & _frame)
    {
        paramFrameIn = _frame;
    }
    //--------------------------------------------------------------
    void VideoSwitcher::newVideoFrame2(VideoFrame & _frame)
    {
        paramFrameIn2 = _frame;
    }

}
