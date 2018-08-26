
#include "VideoRendererWindowNodeBased.h"


namespace ofxPm
{
    VideoRendererWindowNodeBased::VideoRendererWindowNodeBased():ofxOceanodeNodeModelExternalWindow("Video Renderer Window")
    {
        setupNodeBased();
    }


    //--------------------------------------------------------------
    VideoRendererWindowNodeBased::~VideoRendererWindowNodeBased()
    {
        
        //ofRemoveListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }


    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::setupNodeBased()
    {
        color = ofColor::yellow;

        vFrame = ofxPm::VideoFrame();
        
        parameters->add(paramFrameIn.set("Frame In", vFrame));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));

                        
        paramFrameIn.addListener(this, &VideoRendererWindowNodeBased::newVideoFrame);
        
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
        ofSetColor(255,255,0);
        ofDrawRectangle(0,0,w-10,h-10);
        if(!vFrame.isNull() && !vFrame.isNullPtr())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                ofSetColor(255.0*paramOpacity);
                //ofSetColor(128,128,128,128);
                vFrame.getTextureRef().draw(x,y,w,h);
                ofDrawCircle(0,0,100);
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
    void VideoRendererWindowNodeBased::keyPressed(ofKeyEventArgs &a)
    {
        
    }

    //--------------------------------------------------------------
    void VideoRendererWindowNodeBased::mousePressed(ofMouseEventArgs &a)
    {
        
    }

    



}



