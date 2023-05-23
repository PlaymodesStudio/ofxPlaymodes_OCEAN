
#include "VideoRendererNodeBased.h"
#include "imgui.h"


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

        addParameter(paramFrameIn.set("Frame In", VideoFrame()));
        addParameter(paramResolutionX.set("Resolution X",320,0,1920));
        addParameter(paramResolutionY.set("Resolution Y",240,0,1080));
        addParameter(paramPositionX.set("Position X",0,0,1920));
        addParameter(paramPositionY.set("Position Y",0,0,1080));
        addParameter(paramOpacity.set("Opacity",1.0,0.0,1.0));

                        
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
        ImGui::Begin(getParameterGroup().getName().c_str());
        if(!vFrame.isNull())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                ofSetColor(255*paramOpacity);
//                vFrame.getTextureRef().draw(x,y,w,h);
                auto size2 = ImGui::GetContentRegionAvail();
                
                ImTextureID textureID = (ImTextureID)(uintptr_t)vFrame.getTextureRef().texData.textureID;
                ImGui::Image(textureID, size2);
            }
        }
        ImGui::End();
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



