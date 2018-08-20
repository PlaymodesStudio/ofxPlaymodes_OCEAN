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

        source = NULL;
        fps = -1;
        
        string shaderName = "shaders/gamma";
        shader.load(shaderName);
        cout << "GammaFilter::Loading Shader : " << shaderName << endl;
        // allocate fbo where to draw
        if (fbo.isAllocated())
        {
            fbo.allocate(source->getWidth(),source->getHeight(),GL_RGBA);
        }
                
        parameters->add(paramFrameIn.set("Frame Input", frame));
        parameters->add(paramMin.set("Min",0,0.0,1.0));
        parameters->add(paramMax.set("Max",1.0,0.0,1.0));
        parameters->add(paramGamma.set("Gamma",1.0,0.0,4.0));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramMin.addListener(this, &GammaFilter::setMin);
        paramMax.addListener(this, &GammaFilter::setMax);
        
        paramFrameIn.addListener(this, &GammaFilter::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void GammaFilter::update(ofEventArgs &e)
    {
        if(fboHasToBeAllocated != glm::vec2(-1, -1))
        {
            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
            fboHasToBeAllocated = glm::vec2(-1, -1);
        }
    }
    

    //--------------------------------------------------------------
    //VideoFrame LumaFilterNodeBased::getNextVideoFrame()
    //{
    /*
        if(source->getNextVideoFrame()!=NULL)
        {
            return source->getNextVideoFrame();
        }
        return frame;
     */
    //}

    //--------------------------------------------------------------
    void GammaFilter::newVideoFrame(VideoFrame & _frame)
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
                
                frame = VideoFrame::newVideoFrame(fbo);
            }
        }
        paramFrameOut = frame;
    }
}
