
#include "VideoRendererNodeBased.h"
#include "ofxOceanodeContainer.h"


namespace ofxPm
{
    VideoRendererNodeBased::VideoRendererNodeBased():ofxOceanodeNodeModel("Video Renderer")
    {
        setup();
    }


    //--------------------------------------------------------------
    VideoRendererNodeBased::~VideoRendererNodeBased()
    {
        
        //ofRemoveListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }


    //--------------------------------------------------------------
    void VideoRendererNodeBased::setup()
    {

        vFrame = ofxPm::VideoFrame();
        
        parameters->add(paramFrameIn.set("Frame In", vFrame));
        parameters->add(paramResolutionX.set("Resolution X",640,0,1920));
        parameters->add(paramResolutionY.set("Resolution Y",480,0,1080));
        parameters->add(paramPositionX.set("Position X",0,0,1920));
        parameters->add(paramPositionY.set("Position Y",0,0,1080));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));

                        
        paramFrameIn.addListener(this, &VideoRendererNodeBased::newVideoFrame);
        
        //ofAddListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }
    
    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* VideoRendererNodeBased::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoFrame>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoFrame>(), sink.cast<ofxPm::VideoFrame>());
        }
        return nullptr;
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



