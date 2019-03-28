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
        fps = -1;
        
        string shaderName = "shaders/kaleidoscopy";
        shader.load(shaderName);
        cout << "kaleidoscopyNodeBased::Loading Shader : " << shaderName << endl;
        
        parameters->add(paramFrameIn.set("Frame Input", VideoFrame()));

        parameters->add(paramSides.set("Sides",2,0,24));
        parameters->add(paramAngle.set("Angle",0.0,0.0,1.0));
        parameters->add(paramRotation.set("Rotation", 0.0, 0.0, 1.0));
        parameters->add(paramSlideX.set("Slide x",0.5,0.0,1.0));
        parameters->add(paramSlideY.set("Slide y",0.5,0.0,1.0));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        
        listener = paramFrameIn.newListener(this, &kaleidoscopeNodeBased::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void kaleidoscopeNodeBased::update(ofEventArgs &e)
    {
    }
    
    //--------------------------------------------------------------
    void kaleidoscopeNodeBased::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        
        if(!frameIsNull)
        {
            int w = _frame.getWidth();
            int h = _frame.getHeight();
            
            if (!isAllocated || _frame.getWidth() != fbo.getWidth() || _frame.getHeight() != fbo.getHeight())
            {
                fbo.allocate(w, h);
            }
            
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
                            shader.setUniform1f("rotation", paramRotation);
                            shader.setUniform1f("slidex",fmod(paramSlideX+0.5,1.0));
                            shader.setUniform1f("slidey",fmod(paramSlideY+0.5,1.0));

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
                
                paramFrameOut = VideoFrame::newVideoFrame(fbo);
            }
        }
    }
}
