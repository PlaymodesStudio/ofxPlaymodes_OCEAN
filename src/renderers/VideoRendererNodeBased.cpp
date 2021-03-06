
#include "VideoRendererNodeBased.h"


namespace ofxPm
{
    VideoRendererNodeBased::VideoRendererNodeBased():ofxOceanodeNodeModel("Video Renderer")
    {
        //setup();
    }


    //--------------------------------------------------------------
    VideoRendererNodeBased::~VideoRendererNodeBased()
    {
        
        //ofRemoveListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }


    //--------------------------------------------------------------
    void VideoRendererNodeBased::setup()
    {
        color = ofColor::yellow;

        parameters->add(paramFrameIn.set("Frame In", VideoFrame()));
        parameters->add(paramResolutionX.set("Resolution X",320,0,1920));
        parameters->add(paramResolutionY.set("Resolution Y",240,0,1080));
        parameters->add(paramPositionX.set("Position X",0,0,1920));
        parameters->add(paramPositionY.set("Position Y",0,0,1080));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));

                        
        frameInListener = paramFrameIn.newListener(this, &VideoRendererNodeBased::newVideoFrame);
        
        //ofnewListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }


    //--------------------------------------------------------------

    void VideoRendererNodeBased::newVideoFrame(VideoFrame & frame)
    {
        vFrame = frame;
    }
    

    //--------------------------------------------------------------
    void VideoRendererNodeBased::draw(int x,int y,int w,int h)
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
    //--------------------------------------------------------------
    void VideoRendererNodeBased::draw()
    {
        draw(paramPositionX,paramPositionY,paramResolutionX,paramResolutionY);
    }
    //--------------------------------------------------------------
    void VideoRendererNodeBased::draw(ofEventArgs &e)
    {
        draw();
    }
    //--------------------------------------------------------------
    void VideoRendererNodeBased::draw(int _x,int _y)
    {
        if(!vFrame.isNull())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                vFrame.getTextureRef().draw(_x,_y,vFrame.getWidth(),vFrame.getHeight());
            }
        }
    }
    

//--------------------------------------------------------------
ofTexture VideoRendererNodeBased::getLastFrameTexture()
{
    if(!vFrame.isNull())
    {
        if(vFrame.getTextureRef().isAllocated())
        {
            return vFrame.getTextureRef();
        }
        else
        {
            cout << "VideoRenderer :: getLastFrame texture not allocated !!" << endl;
        }
    }
    return vFrame.getTextureRef();;
}

    
    
    



}



