
#include "VideoTrioRendererNodeBased.h"


namespace ofxPm
{
    VideoTrioRendererNodeBased::VideoTrioRendererNodeBased():ofxOceanodeNodeModelExternalWindow("Video Trio Window")
    {
        setup();
    }
    
    
    //--------------------------------------------------------------
    VideoTrioRendererNodeBased::~VideoTrioRendererNodeBased()
    {
        
        //ofRemoveListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }
    
    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::setup()
    {
        
        vFrame = ofxPm::VideoFrame();
        vFrame2 = ofxPm::VideoFrame();
        vFrame3 = ofxPm::VideoFrame();

        parameters->add(paramFrameIn.set("Frame In 1", vFrame));
        parameters->add(paramFrameIn2.set("Frame In 2", vFrame2));
        parameters->add(paramFrameIn3.set("Frame In 3", vFrame3));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));
        parameters->add(paramScale.set("Scale",1.0,0.0,2.0));
        parameters->add(paramOverlap.set("Overlap",0.0,-1.0,1.0));
        parameters->add(paramMinMaxBlend.set("MinMax Blend",true));

        ofParameter<char> pc("Edge Blend",' ');
        parameters->add(pc);
        ofParameterGroup* p = edgeBlend.getParameterGroup();
        for(int i=1 ; i<p->size()-1; i++)
        {
            ofAbstractParameter &absParam = p->get(i);
            parameters->add(absParam);
        }
        
        paramFrameIn.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame);
        paramFrameIn2.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame2);
        paramFrameIn3.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame3);

        //ofAddListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
    }

    
    //--------------------------------------------------------------
    
    void VideoTrioRendererNodeBased::newVideoFrame(VideoFrame & frame)
    {
        ofEventArgs e;
        edgeBlend.update(e);
        edgeBlend.newVideoFrame(frame);
        
        vFrame = edgeBlend.getNextVideoFrame();
        
    }
    //--------------------------------------------------------------
    
    void VideoTrioRendererNodeBased::newVideoFrame2(VideoFrame & frame)
    {
        ofEventArgs e;
        edgeBlend.update(e);
        edgeBlend.newVideoFrame(frame);
        
        vFrame2 = edgeBlend.getNextVideoFrame();
    }
    //--------------------------------------------------------------
    
    void VideoTrioRendererNodeBased::newVideoFrame3(VideoFrame & frame)
    {
        ofEventArgs e;
        edgeBlend.update(e);
        edgeBlend.newVideoFrame(frame);
        
        vFrame3 = edgeBlend.getNextVideoFrame();
    }

    
 
    //--------------------------------------------------------------
    ofTexture VideoTrioRendererNodeBased::getLastFrameTexture()
    {
        if(!vFrame.isNull())
        {
            if(vFrame.getTextureRef().isAllocated())
            {
                return vFrame.getTextureRef();
            }
            else
            {
                cout << "VideoTrioRendererWindow :: getLastFrame texture not allocated !!" << endl;
            }
        }
        return vFrame.getTextureRef();;
    }
    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::drawInExternalWindow(ofEventArgs &e)
    {
        draw(0,0,ofGetWidth(),ofGetHeight());
    }
    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::keyPressed(ofKeyEventArgs &a)
    {
        
    }
    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::mousePressed(ofMouseEventArgs &a)
    {
        
    }


//--------------------------------------------------------------
void VideoTrioRendererNodeBased::draw(int x,int y,int w,int h)
{
    ofSetRectMode(OF_RECTMODE_CENTER);
    
    if(!vFrame.isNull()&&!vFrame2.isNull())
    {
        ofVec2f frameResolution = ofVec2f(vFrame.getWidth(),vFrame.getHeight());
        float frameAspectRatio = frameResolution.x / frameResolution.y;

        
        if(paramMinMaxBlend)
        {
            ofBackground(0);
            glBlendEquationEXT(GL_MAX);
        }
        else
        {
            ofBackground(255);
            glBlendEquationEXT(GL_MIN);
        }

        
        ofPushMatrix();
        
        ofTranslate(paramScale*frameResolution.y/2.0,paramScale*frameResolution.x/2.0,0);
        // rotate to portrait
        ofRotate(90,0,0, 1);
        // flip x
        ofRotate(180,1,0,0);
        //ofTranslate(-frameResolution.x/2.0,-frameResolution.y/2.0,0);
        float finalOverlap = paramOverlap*0.75;
        ofSetColor(255*paramOpacity);
        // Left Frame
        vFrame2.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/-(paramScale*frameResolution.y)+(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,paramScale*frameResolution.y);
        // Right Frame
        vFrame2.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/+(paramScale*frameResolution.y)-(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,paramScale*frameResolution.y);        
        // Center Frame
        vFrame.getTextureRef().draw(0,(ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0),paramScale*frameResolution.x,paramScale*frameResolution.y);

        ofPopMatrix();
    }

    ofSetRectMode(OF_RECTMODE_CORNER);
    
    // TO DO : needed ?
    glBlendEquationEXT(GL_ADD);


}
    
}






