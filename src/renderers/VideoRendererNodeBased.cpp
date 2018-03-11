
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
        ofRemoveListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }


    //--------------------------------------------------------------
    void VideoRendererNodeBased::setup()
    {

        vFrame = ofxPm::VideoFrame();
        
        parameters->setName("Video Renderer");
        parameters->add(paramFrameIn.set("Frame In", vFrame));
        parameters->add(vecdeF.set("Vec",{0},{0},{1}));

                        
        paramFrameIn.addListener(this, &VideoRendererNodeBased::newVideoFrame);
        
        ofAddListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }
    
    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* VideoRendererNodeBased::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoFrame>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoFrame>(), sink.cast<ofxPm::VideoFrame>());
        }
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
                vFrame.getTextureRef().draw(x,y,w,h);
            }
        }
    }
    //--------------------------------------------------------------
    void VideoRendererNodeBased::draw()
    {
        if(!vFrame.isNull())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                vFrame.getTextureRef().draw(20,20,vFrame.getWidth(),vFrame.getHeight());
            }
        }
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



