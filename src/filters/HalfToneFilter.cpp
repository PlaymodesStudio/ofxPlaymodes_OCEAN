#include "HalfToneFilter.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    HalfToneFilter::HalfToneFilter():ofxOceanodeNodeModel("HalfTone FX")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    HalfToneFilter::~HalfToneFilter()
    {
        fbo.clear();
        shader.unload();
    }

    //--------------------------------------------------------------
    void HalfToneFilter::setupNodeBased()
    {
        color = ofColor::darkMagenta;
        fps = -1;
        
        string shaderName = "shaders/halfTone";
        cout << "HalfTone::Loading Shader : " << shaderName << endl;
        shader.load(shaderName);
        
        parameters->add(paramFrameIn.set("Frame Input", VideoFrame()));
        parameters->add(paramColorSteps.set("Color Steps",8,0.0,32.0));
        parameters->add(paramScale.set("Scale",0.25,0.0,4.0));
        parameters->add(paramTimeFactor.set("Time Factor",0.03,0.0,4.0));
        parameters->add(paramBlur.set("Blur",1.0,0.0,10.0));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        
        listener = paramFrameIn.newListener(this, &HalfToneFilter::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void HalfToneFilter::update(ofEventArgs &e)
    {
    }
    

    //--------------------------------------------------------------
    VideoFrame HalfToneFilter::getNextVideoFrame()
    {
		return VideoFrame();
    }

    //--------------------------------------------------------------
    void HalfToneFilter::newVideoFrame(VideoFrame & _frame)
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
                        shader.setUniform1f("iTime",ofGetElapsedTimef());
                        shader.setUniform2f("iResolution", _frame.getWidth(), _frame.getHeight());
                        shader.setUniform1f("colorSteps",paramColorSteps);
                        shader.setUniform1f("scale",paramScale);
                        shader.setUniform1f("timeFactor",paramTimeFactor);
                        shader.setUniform1f("blur",paramBlur);

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
