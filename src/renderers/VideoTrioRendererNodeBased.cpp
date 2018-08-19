
#include "VideoTrioRendererNodeBased.h"


namespace ofxPm
{
    VideoTrioRendererNodeBased::VideoTrioRendererNodeBased():ofxOceanodeNodeModelExternalWindow("Video Trio Window")
    {
        //setup();
    }
    
    
    //--------------------------------------------------------------
    VideoTrioRendererNodeBased::~VideoTrioRendererNodeBased()
    {
    }
    
    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::setup()
    {
        color = ofColor::yellow;
        backImage.load("./imgs/1920x1080.png");
        
        vFrame = ofxPm::VideoFrame();
        vFrame2 = ofxPm::VideoFrame();
        vFrame3 = ofxPm::VideoFrame();

        parameters->add(paramFrameIn.set("Frame In 1", vFrame));
        parameters->add(paramFrameIn2.set("Frame In 2", vFrame2));
        parameters->add(paramFrameIn3.set("Frame In 3", vFrame3));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));
        parameters->add(paramScale.set("Scale",1.6076,0.0,2.0));
        parameters->add(paramOverlap.set("Overlap",0.25,-1.0,1.0));
        parameters->add(paramMinMaxBlend.set("MinMax Blend",true));

        ofParameter<char> pc("Edge Blend",' ');
        parameters->add(pc);
        ofParameterGroup* p = edgeBlend.getParameterGroup();
        for(int i=1 ; i<p->size()-1; i++)
        {
            ofAbstractParameter &absParam = p->get(i);
            parameters->add(absParam);
        }
        
        ofParameter<float> &fp = parameters->getFloat("Width");
        fp.set(0.0214);
        
        parameters->add(createDropdownAbstractParameter("Flips", {">>>", "<>>", ">><", "<<<", "<<>", "><<"}, paramFlipMode));
        
        
        paramFrameIn.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame);
        paramFrameIn2.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame2);
        paramFrameIn3.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame3);
        paramFlipMode.addListener(this, &VideoTrioRendererNodeBased::changedFlipMode);
        
        //ofAddListener(ofEvents().draw, this, &VideoRendererNodeBased::draw);
        
        
        // populate flips vector for fliping textre x -1.0 o 1.0
        vector<int> flips3;
        flips3 = {1,1,1};
        flips.push_back(flips3);
        flips3 = {-1,1,1};
        flips.push_back(flips3);
        flips3 = {1,1,-1};
        flips.push_back(flips3);
        flips3 = {-1,-1,-1};
        flips.push_back(flips3);
        flips3 = {-1,-1,1};
        flips.push_back(flips3);
        flips3 = {1,-1,-1};
        flips.push_back(flips3);
    }

    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::showMyExternalWindow(bool b)
    {
        cout << "TrioRender showing window : " << b << endl;
        //ofxOceanodeNodeModelExternalWindow::showExternalWindow(b);
        showWindow = b;
        cout << "ofGetScreenWidth() = " << ofGetScreenWidth() << endl;
        cout << "ofGetScreenHeigth() = " << ofGetScreenHeight() << endl;
        setExternalWindowPosition(ofGetScreenWidth(),0);
        setExternalWindowShape(1920,1080);
        //setExternalWindowFullScreen(true);
    }
    
    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::changedFlipMode(int &m)
    {
//        cout << "flip mode : " << m << endl;
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
    
    void VideoTrioRendererNodeBased::newVideoFrame2(VideoFrame & frame2)
    {
        ofEventArgs e;
        edgeBlend.update(e);
        edgeBlend.newVideoFrame(frame2);
        
        vFrame2 = edgeBlend.getNextVideoFrame();
    }
    //--------------------------------------------------------------
    
    void VideoTrioRendererNodeBased::newVideoFrame3(VideoFrame & frame3)
    {
        ofEventArgs e;
        edgeBlend.update(e);
        edgeBlend.newVideoFrame(frame3);
        
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
        ofSetColor(255);
        backImage.draw(0,0,ofGetWidth(),ofGetHeight());
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
    
    // check wich option to draw based on incoming frames
    bool b1,b2,b3 = false;

    if(!vFrame.isNull())
    {
        b1=true;
    }
    if(!vFrame2.isNull())
    {
        b2=true;
    }
    if(!vFrame3.isNull())
    {
        b3=true;
    }

    if(b1 || b2 || b3)
    {
        
        // start drawing
        ofVec2f frameResolution;
        if(b1)
        {
            frameResolution = ofVec2f(vFrame.getWidth(),vFrame.getHeight());
        }
        else if(b2)
        {
            frameResolution = ofVec2f(vFrame2.getWidth(),vFrame2.getHeight());
        }
        else if(b3)
        {
            frameResolution = ofVec2f(vFrame3.getWidth(),vFrame3.getHeight());
        }

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
//        ofTranslate(paramScale*frameResolution.y/2.0,paramScale*frameResolution.x/2.0,0);
//        // rotate to portrait
//        ofRotate(90,0,0, 1);
//        // flip x
//        ofRotate(180,1,0,0);
//        //ofTranslate(-frameResolution.x/2.0,-frameResolution.y/2.0,0);
        float finalOverlap = paramOverlap*0.75;
        ofSetColor(255*paramOpacity);

        
        if(b1 && !b2 && !b3)
        {
            // just draw v1 in 3 positions
            // Left Frame
            vFrame.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/-(paramScale*frameResolution.y)+(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][0]*paramScale*frameResolution.y);
            // Center Frame
            vFrame.getTextureRef().draw(0,(ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0),paramScale*frameResolution.x,flips[paramFlipMode][1]*paramScale*frameResolution.y);
            // Right Frame
            vFrame.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/+(paramScale*frameResolution.y)-(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][2]*paramScale*frameResolution.y);

        }
        else if(b1 && b2 && !b3)
        {
            // draw v1 in center and v2 in sides
            // Left Frame
            vFrame2.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/-(paramScale*frameResolution.y)+(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][0]*paramScale*frameResolution.y);
            // Center Frame
            vFrame.getTextureRef().draw(0,(ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0),paramScale*frameResolution.x,flips[paramFlipMode][1]*paramScale*frameResolution.y);
            // Right Frame
            vFrame2.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/+(paramScale*frameResolution.y)-(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][2]*paramScale*frameResolution.y);
        }
        else if(b1 && b2 && b3)
        {
            // draw v1 center, v2 left, v3 right
            // draw v1 in center and v2 in sides
            // Left Frame
            vFrame2.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/-(paramScale*frameResolution.y)+(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][0]*paramScale*frameResolution.y);
            // Center Frame
            vFrame.getTextureRef().draw(0,(ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0),paramScale*frameResolution.x,flips[paramFlipMode][1]*paramScale*frameResolution.y);
            // Right Frame
            vFrame3.getTextureRef().draw(0,(/**/ofGetWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/+(paramScale*frameResolution.y)-(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][2]*paramScale*frameResolution.y);
        }

        ofPopMatrix();
    }
    ofSetRectMode(OF_RECTMODE_CORNER);
    // TO DO : needed ?
    glBlendEquationEXT(GL_ADD);


}
    
}






