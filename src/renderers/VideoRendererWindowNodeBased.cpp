
#include "VideoRendererWindowNodeBased.h"


namespace ofxPm
{
    VideoRendererWindowNodeBased::VideoRendererWindowNodeBased():ofxOceanodeNodeModelExternalWindow("Video Renderer Window")
    {
        setup();
    }


    //--------------------------------------------------------------
    VideoRendererWindowNodeBased::~VideoRendererWindowNodeBased()
    {
        
        //ofRemoveListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }


    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::setup()
    {

        vFrame = ofxPm::VideoFrame();
        
        parameters->add(paramFrameIn.set("Frame In", vFrame));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));

                        
        paramFrameIn.addListener(this, &VideoRendererWindowNodeBased::newVideoFrame);
        
        //ofAddListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }

    //--------------------------------------------------------------

    void VideoRendererWindowNodeBased::newVideoFrame(VideoFrame & frame)
    {
        vFrame = frame;
    }
    

    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::draw(int x,int y,int w,int h)
    {
        if(!vFrame.isNull())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                ofSetColor(255*paramOpacity);
                vFrame.getTextureRef().draw(x,y,w,h);
            }
        }
    }
//    //--------------------------------------------------------------
//    void VideoRendererWindowNodeBased::draw()
//    {
//        draw(paramPositionX,paramPositionY,paramResolutionX,paramResolutionY);
//        ofSetColor(0,255,0);
//        ofDrawRectangle(0,0,100,100);
//    }
//    //--------------------------------------------------------------
//    void VideoRendererWindowNodeBased::draw(ofEventArgs &e)
//    {
//        draw();
//    }
//    //--------------------------------------------------------------
//    void VideoRendererWindowNodeBased::draw(int _x,int _y)
//    {
//        if(!vFrame.isNull())
//        {
//            if(vFrame.getTextureRef().isAllocated())
//            {
//                vFrame.getTextureRef().draw(_x,_y,vFrame.getWidth(),vFrame.getHeight());
//            }
//        }
//    }
    

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
    void VideoRendererWindowNodeBased::keyPressed(ofKeyEventArgs &a)
    {
        
    }

    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::mousePressed(ofMouseEventArgs &a)
    {
        
    }

    



}



