
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
        
        parameters->add(paramFrameIn.set("Frame Input 1", VideoFrame()));
        parameters->add(paramFrameIn2.set("Frame Input 2", VideoFrame()));
        parameters->add(paramSwitchTo.set("Switch",0,0,1));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        
        paramFrameIn.addListener(this, &VideoSwitcher::newVideoFrame);
        paramFrameIn2.addListener(this, &VideoSwitcher::newVideoFrame2);

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
