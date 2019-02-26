
#include "VideoTrioRendererNodeBased.h"


namespace ofxPm
{
    VideoTrioRendererNodeBased::VideoTrioRendererNodeBased():ofxOceanodeNodeModel("Video Trio Renderer")
    {
    }
    
    
    //--------------------------------------------------------------
    VideoTrioRendererNodeBased::~VideoTrioRendererNodeBased()
    {
    }
    
    
    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::setup()
    {
        color = ofColor::yellow;
     
//        VideoFrame vA;
//        VideoFrame vB;
        
        // setup black frame
        ofPixels pix;
        pix.allocate(1, 1,3);
        for(int i=0;i<1*1*3;i++)
        {
            pix[i] = 0;
        }
        //blackFrame = VideoFrame::newVideoFrame(pix);
        parameters->add(paramFrameIn.set("Frame In 1", VideoFrame::newVideoFrame(pix)));
        parameters->add(paramFrameIn2.set("Frame In 2", VideoFrame::newVideoFrame(pix)));
        parameters->add(paramOpacity.set("Opacity",1.0,0.0,1.0));
        parameters->add(paramScale.set("Scale",1.6076,0.0,2.0));
        parameters->add(paramOverlap.set("Overlap",0.25,-1.0,1.0));
        parameters->add(paramMinMaxBlend.set("MinMax Blend",true));
        
//        // EDGE BLEND
//        ofParameter<char> pc("Edge Blend",' ');
//        parameters->add(pc);
//        ofParameterGroup* p = edgeBlend.getParameterGroup();
//        for(int i=1 ; i<p->size()-1; i++)
//        {
//            ofAbstractParameter &absParam = p->get(i);
//            parameters->add(absParam);
//        }
//        ofParameter<float> &fp = parameters->getFloat("Width");
//        fp.set(0.0214);
        
        // FLIPS
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
        parameters->add(createDropdownAbstractParameter("Flips", {">>>", "<>>", ">><", "<<<", "<<>", "><<"}, paramFlipMode));

        // LAYOUTS
        // populate layout vector for output
        vector<int> layout3;
        layout3 = {1,1,1};
        layouts.push_back(layout3);
        layout3 = {2,1,2};
        layouts.push_back(layout3);
        layout3 = {1,2,1};
        layouts.push_back(layout3);
        layout3 = {2,2,2};
        layouts.push_back(layout3);
        parameters->add(createDropdownAbstractParameter("Layout", {"1-1-1", "2-1-2", "1-2-1", "2-2-2"}, paramLayout));
        parameters->add(paramFrameOut.set("Frame Out", VideoFrame()));
        
        paramFrameIn.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame);
        paramFrameIn2.addListener(this, &VideoTrioRendererNodeBased::newVideoFrame2);
        paramFlipMode.addListener(this, &VideoTrioRendererNodeBased::changedFlipMode);
     
        frameRefCentre = new VideoFrame();
        frameRefDreta = new VideoFrame();;
        frameRefEsquerra = new VideoFrame();
        

    }

    //--------------------------------------------------------------
    void VideoTrioRendererNodeBased::changedFlipMode(int &m)
    {
    }

    //--------------------------------------------------------------

    void VideoTrioRendererNodeBased::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        
        if(!frameIsNull)
        {
            int w = 1920;
            int h = 1080;

            if (!isAllocated || w != fbo.getWidth() || h != fbo.getHeight())
            {
                fbo.allocate(w,h);
            }
            vFrame = _frame;
            drawIntoFbo(0,0,0,0);

        }
        
    }
    //--------------------------------------------------------------
    
    void VideoTrioRendererNodeBased::newVideoFrame2(VideoFrame & _frame2)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame2.isNull();
        
        if(!frameIsNull)
        {
            int w = 1920;
            int h = 1080;
            
            if (!isAllocated || w != fbo.getWidth() || h != fbo.getHeight())
            {
                fbo.allocate(w, h);
            }
            vFrame2 = _frame2;
            if(vFrame.isNullPtr() || vFrame.getFboRef().getWidth()==1)
            {
               drawIntoFbo(0,0,0,0);
            }
        }
        
    }
    
    //--------------------------------------------------------------
    ofTexture VideoTrioRendererNodeBased::getLastFrameTexture()
    {
//        cout << "_TRIO LST:" <<  ofGetWindowPtr()->getNSGLContext() << endl;

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
void VideoTrioRendererNodeBased::drawIntoFbo(int x,int y,int w,int h)
{
    fbo.begin();

    // initialize intermediate VideoFrames to NULL
    frameRefCentre = NULL;
    frameRefDreta = NULL;
    frameRefEsquerra = NULL;
    
    ofVec2f frameResolution,frameResolution1,frameResolution2;
    bool frame1OK = (!vFrame.isNull() && !vFrame.isNullPtr() && vFrame.getFboRef().getWidth()!=1);
    bool frame2OK = (!vFrame2.isNull() && !vFrame2.isNullPtr() && vFrame2.getFboRef().getWidth()!=1);
    
    switch(paramLayout.get())
    {
        case 0 : // 1 1 1
            if(frame1OK)
            {
                frameRefEsquerra = &vFrame;
                frameRefCentre = &vFrame;
                frameRefDreta = &vFrame;
                frameResolution = ofVec2f(vFrame.getWidth(),vFrame.getHeight());
            }
            break;
            
        case 1 : // 2 1 2
            if(frame1OK)
            {
                frameRefCentre = &vFrame;
                frameResolution = ofVec2f(vFrame.getWidth(),vFrame.getHeight());
            }
            if(frame2OK)
            {
                frameRefDreta = &vFrame2;
                frameRefEsquerra = &vFrame2;
                frameResolution2 = ofVec2f(vFrame2.getWidth(),vFrame2.getHeight());
            }
            if(frame2OK && !frame1OK)
            {
                frameResolution = frameResolution2;
            }
            break;
            
        case 2 : // 1 2 1
            if(frame1OK)
            {
                frameRefDreta = &vFrame;
                frameRefEsquerra = &vFrame;
                frameResolution = ofVec2f(vFrame.getWidth(),vFrame.getHeight());
            }
            if(frame2OK)
            {
                frameRefCentre = &vFrame2;
                frameResolution2 = ofVec2f(vFrame2.getWidth(),vFrame2.getHeight());
                
            }
            if(frame2OK && !frame1OK)
            {
                frameResolution = frameResolution2;
            }
            break;
            
        case 3 : // 2 2 2
            if(frame2OK)
            {
                frameRefEsquerra = &vFrame2;
                frameRefCentre = &vFrame2;
                frameRefDreta = &vFrame2;
                frameResolution2 = ofVec2f(vFrame2.getWidth(),vFrame2.getHeight());
                frameResolution = frameResolution2;
            }
            break;
            
    }
    
    if(frame1OK || frame2OK)
    {
        ofSetRectMode(OF_RECTMODE_CENTER);
        
        // MIN MAX BLENDING
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
        {
            // TRANSFORM FRAME
            ofTranslate(paramScale*frameResolution.y/2.0,paramScale*frameResolution.x/2.0,0);
            // rotate to portrait
            ofRotate(90,0,0, 1);
            // flip x
            ofRotate(180,1,0,0);
            float finalOverlap = paramOverlap*0.75;
            ofSetColor(255*paramOpacity);
            
            // DRAWING FRAMES
            if(frameRefEsquerra != NULL)
            {
                // Left Frame
                if(!frameRefEsquerra->isNull() || !frameRefEsquerra->isNullPtr())
                {
                    frameRefEsquerra->getTextureRef().draw(0,(/**/fbo.getWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/-(paramScale*frameResolution.y)+(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][0]*paramScale*frameResolution.y);
                }
            }
            if(frameRefCentre!=NULL)
            {
                // Center Frame
                if(!frameRefCentre->isNull() || !frameRefCentre->isNullPtr())
                {
                    frameRefCentre->getTextureRef().draw(0,(fbo.getWidth()/2.0)-((paramScale*frameResolution.y)/2.0),paramScale*frameResolution.x,flips[paramFlipMode][1]*paramScale*frameResolution.y);
                }
            }
            if(frameRefDreta!=NULL)
            {
                if(!frameRefDreta->isNull() && !frameRefDreta->isNullPtr())
                {
                    // Right Frame
                    frameRefDreta->getTextureRef().draw(0,(/**/fbo.getWidth()/2.0)-((paramScale*frameResolution.y)/2.0)/**/+(paramScale*frameResolution.y)-(finalOverlap*paramScale*frameResolution.x),paramScale*frameResolution.x,flips[paramFlipMode][2]*paramScale*frameResolution.y);
                    
                }
            }
            ofPopMatrix();
        }
        ofSetRectMode(OF_RECTMODE_CORNER);
        glBlendEquationEXT(GL_ADD);

    }
    fbo.end();
    
    paramFrameOut = VideoFrame::newVideoFrame(fbo);


}

}


