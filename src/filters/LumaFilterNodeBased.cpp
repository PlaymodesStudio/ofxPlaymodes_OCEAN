#include "LumaFilterNodeBased.h"
#include "ofxOceanodeContainer.h"

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
        lumaSmooth=0.25;
        lumaThreshold=0.025;
        source = NULL;
        fps = -1;
        
        string shaderName = "shaders/lumakey";
        shader.load(shaderName);
        cout << "LumaFilter::Loading Shader : " << shaderName << endl;
        // allocate fbo where to draw
        if (fbo.isAllocated())
        {
            fbo.allocate(source->getWidth(),source->getHeight(),GL_RGBA);
        }
                
        parameters->add(paramFrameIn.set("Frame Input", frame));
        parameters->add(paramLumaThrshold.set("Threshold",0.25,0.0,1.0));
        parameters->add(paramLumaSmooth.set("Smooth",0.25,0.0,1.0));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramLumaThrshold.addListener(this, &LumaFilterNodeBased::setLumaThreshold);
        paramLumaSmooth.addListener(this, &LumaFilterNodeBased::setLumaSmooth);
        
        paramFrameIn.addListener(this, &LumaFilterNodeBased::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void LumaFilterNodeBased::update(ofEventArgs &e)
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
    void LumaFilterNodeBased::newVideoFrame(VideoFrame & _frame)
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
                        shader.setUniform1f("u_smooth",lumaSmooth);
                        shader.setUniform1f("u_max",lumaThreshold);
                        
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

    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* LumaFilterNodeBased::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoFrame>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoFrame>(), sink.cast<ofxPm::VideoFrame>());
        }
        return nullptr;
    }

}
