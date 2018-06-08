#include "ColorCorrectFilter.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    ColorCorrectFilter::ColorCorrectFilter():ofxOceanodeNodeModel("Color Balance FX")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    ColorCorrectFilter::~ColorCorrectFilter()
    {
        fbo.clear();
        shader.unload();
    }

    //--------------------------------------------------------------
    void ColorCorrectFilter::setupNodeBased()
    {
        source = NULL;
        fps = -1;
        
        string shaderName = "shaders/colorBalance";
        shader.load(shaderName);
        cout << "ColorCorrect::Loading Shader : " << shaderName << endl;
        // allocate fbo where to draw
        if (fbo.isAllocated())
        {
            fbo.allocate(source->getWidth(),source->getHeight(),GL_RGBA);
        }
                
        parameters->add(paramFrameIn.set("Frame Input", frame));
        parameters->add(paramRed.set("Red",1.0,0.0,10.0));
        parameters->add(paramGreen.set("Green",1.0,0.0,10.0));
        parameters->add(paramBlue.set("Blue",1.0,0.0,10.0));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
//        paramLumaThrshold.addListener(this, &ColorCorrectFilter::setLumaThreshold);
        
        paramFrameIn.addListener(this, &ColorCorrectFilter::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void ColorCorrectFilter::update(ofEventArgs &e)
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
    void ColorCorrectFilter::newVideoFrame(VideoFrame & _frame)
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
                        
                        shader.setUniform1f("u_red",paramRed);
                        shader.setUniform1f("u_green",paramGreen);
                        shader.setUniform1f("u_blue",paramBlue);

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
