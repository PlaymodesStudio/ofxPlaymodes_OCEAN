#include "inputProcessingFilter.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    inputProcessingFilter::inputProcessingFilter():ofxOceanodeNodeModel("Input Processing")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    inputProcessingFilter::~inputProcessingFilter()
    {
        fbo.clear();
        shader.unload();
    }

    //--------------------------------------------------------------
    void inputProcessingFilter::setupNodeBased()
    {
        color = ofColor::darkMagenta;
        fps = -1;
        
        string shaderName = "shaders/inputProcessing";
        cout << "inputProcessingFilter::Loading Shader : " << shaderName << endl;
        shader.load(shaderName);
        
        parameters->add(paramFrameIn.set("Frame Input", frame));
        ofParameter<char> lk("Luma Key",' ');
        parameters->add(lk);
        parameters->add(paramLumaThrshold.set("Threshold",0.25,0.0,1.0));
        parameters->add(paramLumaSmooth.set("Smooth",0.25,0.0,1.0));
        ofParameter<char> gg("Gradient",' ');
        parameters->add(gg);
        parameters->add(paramGradientWidth.set("Width",0.25,0.0,1.0));
        parameters->add(paramGradientXorY.set("X or Y",1,0,1));
        ofParameter<char> ll("Levels",' ');
        parameters->add(ll);
        parameters->add(paramMin.set("Min",0,0.0,1.0));
        parameters->add(paramMax.set("Max",1.0,0.0,1.0));
        parameters->add(paramGamma.set("Gamma",1.0,0.0,4.0));

        parameters->add(paramFrameOut.set("Frame Output", frame));
                
        listeners.push(paramLumaThrshold.newListener(this, &inputProcessingFilter::setLumaThreshold));
        listeners.push(paramLumaSmooth.newListener(this, &inputProcessingFilter::setLumaSmooth));
        listeners.push(paramGradientWidth.newListener(this, &inputProcessingFilter::setGradientWidth));
        listeners.push(paramGradientXorY.newListener(this, &inputProcessingFilter::setGradientXorY));
        listeners.push(paramFrameIn.newListener(this, &inputProcessingFilter::newVideoFrame));
        
    }
    //------------------------------------------------------------
    void inputProcessingFilter::update(ofEventArgs &e)
    {
//        if(fboHasToBeAllocated != glm::vec2(-1, -1))
//        {
//            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
//            fboHasToBeAllocated = glm::vec2(-1, -1);
//        }
    }
    
    //------------------------------------------------------------
    VideoFrame inputProcessingFilter::getNextVideoFrame()
    {
        return frame;
        
    }
    //--------------------------------------------------------------
    void inputProcessingFilter::newVideoFrame(VideoFrame & _frame)
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
                        shader.setUniform1f("u_width",paramGradientWidth);
                        shader.setUniform1i("u_xory",paramGradientXorY);
                        shader.setUniform1f("u_smooth",paramLumaSmooth);
                        shader.setUniform1f("u_max",paramLumaThrshold);
                        
                        shader.setUniform1f("u_LevMin",paramMin);
                        shader.setUniform1f("u_LevMax",paramMax);
                        shader.setUniform1f("u_LevGamma",paramGamma);

                        ofSetColor(255);
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
