#include "kaleidoscopeNodeBased.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    kaleidoscopeNodeBased::kaleidoscopeNodeBased():ofxOceanodeNodeModel("Kaleidoscope FX")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    kaleidoscopeNodeBased::~kaleidoscopeNodeBased()
    {
        fbo.clear();
        shader.unload();
    }

    //--------------------------------------------------------------
    void kaleidoscopeNodeBased::setupNodeBased()
    {
        color = ofColor::darkMagenta;

        lumaSmooth=0.25;
        lumaThreshold=0.025;
        source = NULL;
        fps = -1;
        
        string shaderName = "shaders/kaleidoscopy";
        shader.load(shaderName);
        cout << "kaleidoscopyNodeBased::Loading Shader : " << shaderName << endl;
        // allocate fbo where to draw
        if (fbo.isAllocated())
        {
            fbo.allocate(source->getWidth(),source->getHeight(),GL_RGBA);
        }
                
        parameters->add(paramFrameIn.set("Frame Input", frame));

        parameters->add(paramSides.set("Sides",2,0,24));
        parameters->add(paramAngle.set("Angle",0.0,-1.0,1.0));
        parameters->add(paramSlideX.set("Slide x",0.5,0.0,1.0));
        parameters->add(paramSlideY.set("Slide y",0.5,0.0,1.0));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramFrameIn.addListener(this, &kaleidoscopeNodeBased::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void kaleidoscopeNodeBased::update(ofEventArgs &e)
    {
        if(fboHasToBeAllocated != glm::vec2(-1, -1))
        {
            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
            fboHasToBeAllocated = glm::vec2(-1, -1);
        }
    }
    
    //--------------------------------------------------------------
    void kaleidoscopeNodeBased::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        
        if(!frameIsNull)
        {
            if (!isAllocated || _frame.getWidth() != fbo.getWidth() || _frame.getHeight() != fbo.getHeight())
            {
                fboHasToBeAllocated = glm::vec2(_frame.getWidth(), _frame.getHeight());
            }

            if(fbo.isAllocated())
            {
                fbo.begin();
                {
                    ofClear(0, 0, 0, 255);
                    int w = _frame.getWidth();
                    int h = _frame.getHeight();

                    if(paramSides>0)
                    {
                        shader.begin();
                        {
                            shader.setUniformTexture("tex0",_frame.getTextureRef(),11);
                            shader.setUniform1f("sides",paramSides);
                            shader.setUniform1f("angle",paramAngle);
                            shader.setUniform1f("slidex",paramSlideX);
                            shader.setUniform1f("slidey",paramSlideY);

                            ofSetColor(255);
                            
                            _frame.getTextureRef().draw(0,0,w,h);
                        }
                        shader.end();
                    }
                    else
                    {
                        ofSetColor(255);
                        _frame.getTextureRef().draw(0,0,w,h);
                    }
                }
                fbo.end();
                
                frame = VideoFrame::newVideoFrame(fbo);
            }
        }
        paramFrameOut = frame;
    }
}
