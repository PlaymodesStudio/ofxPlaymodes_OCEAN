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
        color = ofColor::darkMagenta;
        fps = -1;
        
        string shaderName = "shaders/colorBalance";
        shader.load(shaderName);
        cout << "ColorCorrect::Loading Shader : " << shaderName << endl;
        
        addParameter(paramFrameIn.set("Frame Input", VideoFrame()));
        addParameter(paramRed.set("Red",1.0,0.0,10.0));
        addParameter(paramGreen.set("Green",1.0,0.0,10.0));
        addParameter(paramBlue.set("Blue",1.0,0.0,10.0));
        addParameter(paramFrameOut.set("Frame Output", VideoFrame()));
        
        listener = paramFrameIn.newListener(this, &ColorCorrectFilter::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void ColorCorrectFilter::update(ofEventArgs &e)
    {
    }
    
    //--------------------------------------------------------------
    void ColorCorrectFilter::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        
        if(!frameIsNull)
        {
            int w = _frame.getWidth();
            int h = _frame.getHeight();
            
            if (!isAllocated || w != fbo.getWidth() || h != fbo.getHeight())
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
                        shader.setUniform1f("u_red",paramRed);
                        shader.setUniform1f("u_green",paramGreen);
                        shader.setUniform1f("u_blue",paramBlue);

                        ofSetColor(255);
                        int w = _frame.getWidth();
                        int h = _frame.getHeight();
                        
                        _frame.getTextureRef().draw(0,0,w,h);
                        //_frame.getTextureRef().unbind();
                    }
                    shader.end();
                }
                fbo.end();
                
                paramFrameOut = VideoFrame::newVideoFrame(fbo);
                //paramFrameOut = VideoFrame(fbo);
            }
        }
    }
}
