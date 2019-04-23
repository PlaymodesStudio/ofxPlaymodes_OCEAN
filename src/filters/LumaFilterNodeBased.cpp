#include "LumaFilterNodeBased.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    LumaFilterNodeBased::LumaFilterNodeBased():ofxOceanodeNodeModel("LumaKey FX")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    LumaFilterNodeBased::~LumaFilterNodeBased()
    {
        fbo.clear();
        shader.unload();
    }

    //--------------------------------------------------------------
    void LumaFilterNodeBased::setupNodeBased()
    {
        color = ofColor::darkMagenta;
        fps = -1;
        
        string shaderName = "shaders/lumakey";
        shader.load(shaderName);
        cout << "LumaFilter::Loading Shader : " << shaderName << endl;
        
        parameters->add(paramFrameIn.set("Frame Input", VideoFrame()));
        addParameterToGroupAndInfo(paramLumaThrshold.set("Threshold",0.25,0.0,1.0)).isSavePreset = false;
        addParameterToGroupAndInfo(paramLumaSmooth.set("Smooth",0.25,0.0,1.0)).isSavePreset = false;
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        
        listeners.push(paramLumaThrshold.newListener(this, &LumaFilterNodeBased::setLumaThreshold));
        listeners.push(paramLumaSmooth.newListener(this, &LumaFilterNodeBased::setLumaSmooth));
        
        listeners.push(paramFrameIn.newListener(this, &LumaFilterNodeBased::newVideoFrame));
        
    }
    //------------------------------------------------------------
    void LumaFilterNodeBased::update(ofEventArgs &e)
    {
    }
    

    //--------------------------------------------------------------
    VideoFrame LumaFilterNodeBased::getNextVideoFrame()
    {
        cout << "@@@@@@@@@@@@@@@@@" << endl;
        cout << "@@@@@@@@@@@@@@@@@" << endl;
        cout << "@@@@@@@@@@@@@@@@@" << endl;
        cout << "@@@@@@@@@@@@@@@@@" << endl;
        cout << "@@@@@@@@@@@@@@@@@" << endl;
        cout << "@@@@@@@@@@@@@@@@@" << endl;
		return VideoFrame();
    }

    //--------------------------------------------------------------
    void LumaFilterNodeBased::newVideoFrame(VideoFrame & _frame)
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
                        shader.setUniform1f("u_smooth",paramLumaSmooth);
                        shader.setUniform1f("u_max",paramLumaThrshold);
                        
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
