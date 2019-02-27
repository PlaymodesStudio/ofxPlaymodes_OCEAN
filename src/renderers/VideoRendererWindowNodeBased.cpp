
#include "VideoRendererWindowNodeBased.h"


namespace ofxPm
{
    VideoRendererWindowNodeBased::VideoRendererWindowNodeBased():ofxOceanodeNodeModelExternalWindow("Video Renderer Window")
    {
    }


    //--------------------------------------------------------------
    VideoRendererWindowNodeBased::~VideoRendererWindowNodeBased()
    {
    }


    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::setup()
    {
        color = ofColor::yellow;
        backImage.load("./imgs/1920x1080.png");

        parameters->add(paramFrameIn.set("Frame In", VideoFrame()));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));

                        
        listener = paramFrameIn.newListener(this, &VideoRendererWindowNodeBased::newVideoFrame);
        
        //ofAddListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }

    //--------------------------------------------------------------

    void VideoRendererWindowNodeBased::newVideoFrame(VideoFrame & frame)
    {
//        vFrame = VideoFrame(frame.getTextureRef());
        vFrame = frame;
    }
    

    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::draw(int x,int y,int w,int h)
    {
        backImage.draw(0,0,w,h);
        if(!vFrame.isNull() && !vFrame.isNullPtr())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                ofSetColor(255.0*paramOpacity);
                //ofSetColor(128,128,128,128);
                vFrame.getTextureRef().draw(x,y,w,h);
            }
        }
    }


    //--------------------------------------------------------------
    ofTexture VideoRendererWindowNodeBased::getLastFrameTexture()
    {
        if(!vFrame.isNull())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                return vFrame.getTextureRef();
            }
            else
            {
                cout << "VideoRendererWindow :: getLastFrame texture not allocated !!" << endl;
            }
        }
        return vFrame.getTextureRef();;
    }

    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::drawInExternalWindow(ofEventArgs &e)
    {
        draw(0,0,ofGetWidth(),ofGetHeight());
    }
    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::showMyExternalWindow(bool b)
    {
        //ofxOceanodeNodeModelExternalWindow::showExternalWindow(b);
        showWindow = b;
        setExternalWindowPosition(ofGetScreenWidth(),0);
        setExternalWindowShape(1920,1080);
        //setExternalWindowFullScreen(true);
    }

    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::keyPressed(ofKeyEventArgs &a)
    {
        
    }

    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::mousePressed(ofMouseEventArgs &a)
    {
        
    }

    



}



