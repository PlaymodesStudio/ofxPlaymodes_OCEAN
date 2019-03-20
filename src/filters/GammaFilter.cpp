#include "GammaFilter.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    GammaFilter::GammaFilter():ofxOceanodeNodeModel("Gamma Levels FX")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    GammaFilter::~GammaFilter()
    {
        fbo.clear();
        shader.unload();
    }

    //--------------------------------------------------------------
    void GammaFilter::setupNodeBased()
    {
        color = ofColor::darkMagenta;
        fps = -1;
        
        string shaderName = "shaders/gamma";
        shader.load(shaderName);
        cout << "GammaFilter::Loading Shader : " << shaderName << endl;
        
        parameters->add(paramFrameIn.set("Frame Input", VideoFrame()));
        parameters->add(paramMin.set("Min",0,0.0,1.0));
        parameters->add(paramMax.set("Max",1.0,0.0,1.0));
        parameters->add(paramGamma.set("Gamma",1.0,0.0,4.0));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        
        listeners.push(paramMin.newListener(this, &GammaFilter::setMin));
        listeners.push(paramMax.newListener(this, &GammaFilter::setMax));
        
        listeners.push(paramFrameIn.newListener(this, &GammaFilter::newVideoFrame));
        
    }
    //------------------------------------------------------------
    void GammaFilter::update(ofEventArgs &e)
    {

    }
    
    //--------------------------------------------------------------
    void GammaFilter::newVideoFrame(VideoFrame & _frame)
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
                    shader.begin();
                    {
                        shader.setUniformTexture("tex0",_frame.getTextureRef(),11);
                        shader.setUniform1f("u_min",paramMin);
                        shader.setUniform1f("u_max",paramMax);
                        shader.setUniform1f("u_gamma",paramGamma);

                        ofSetColor(255);
                        int w = _frame.getWidth();
                        int h = _frame.getHeight();
                        
                        _frame.getTextureRef().draw(0,0,w,h);
                    }
                    shader.end();
                }
                fbo.end();
                
                paramFrameOut = VideoFrame::newVideoFrame(fbo);
            }
        }
    }
}
