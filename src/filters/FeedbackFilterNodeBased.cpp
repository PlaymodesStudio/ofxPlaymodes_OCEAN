
#include "FeedbackFilterNodeBased.h"

namespace ofxPm{
    
    //------------------------------------------------------------
    FeedbackFilterNodeBased::FeedbackFilterNodeBased(): ofxOceanodeNodeModel("Feedback FX")
    {
        setupNodeBased();
    }

    //------------------------------------------------------------
    FeedbackFilterNodeBased::~FeedbackFilterNodeBased()
    {
        fbo.clear();
        shader.unload();
    }

    //------------------------------------------------------------
    void FeedbackFilterNodeBased::setupNodeBased(){
        
        color = ofColor::darkMagenta;
        fps = -1;
        
        string shaderName = "shaders/feedback";
        cout << "FeedbackFilter::Loading Shader : " << shaderName << endl;
        shader.load(shaderName);
        
        parameters->add(paramFrameIn.set("Frame Input", frameToSendOut));
        parameters->add(paramScale.set("Scale",0.98,-1.0,2.0));
        parameters->add(paramMixAmmount.set("Mix Ammount",0.1,0.0,1.0));
        parameters->add(paramOriginX.set("OriginX",0.5,-1.0,1.0));
        parameters->add(paramOriginY.set("OriginY",0.5,-1.0,1.0));
        parameters->add(paramFrameOut.set("Frame Output", frameToSendOut));
        
        listener = paramFrameIn.newListener(this, &FeedbackFilterNodeBased::newVideoFrame);
    }

    //------------------------------------------------------------
    void FeedbackFilterNodeBased::update(ofEventArgs &e)
    {
//        if(fboHasToBeAllocated != glm::vec2(-1, -1))
//        {
//            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
//            fboHasToBeAllocated = glm::vec2(-1, -1);
//        }
    }

    //------------------------------------------------------------
    VideoFrame FeedbackFilterNodeBased::getNextVideoFrame()
    {
        return frameToSendOut;

    }

    //------------------------------------------------------------
    void FeedbackFilterNodeBased::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        
        
        if(!frameIsNull)
        {
            // this is for the start of the fx process ... we need to have something on "frame" as the lastframe.
            if(frameToSendOut.isNull())
            {
                frameToSendOut = _frame;
            }

            // 
            if (!isAllocated || (_frame.getWidth() != fbo.getWidth()) || (_frame.getHeight() != fbo.getHeight()) )
            {
//                fboHasToBeAllocated = glm::vec2(_frame.getWidth(), _frame.getHeight());
                fbo.allocate(_frame.getWidth(), _frame.getHeight());
            }

            {
                fbo.begin();
                {
                    ofClear(0, 0, 0, 255);
                    shader.begin();
                    {
                        shader.setUniformTexture("tex0",_frame.getTextureRef(),0);
                        shader.setUniformTexture("tex1",frameToSendOut.getTextureRef(),1);
                        shader.setUniform1f("u_scale",paramScale);
                        shader.setUniform1f("u_mixAmmount",paramMixAmmount);
                        shader.setUniform1f("u_originX",paramOriginX);
                        shader.setUniform1f("u_originY",paramOriginY);
                        float* res = new float[2];
                        res[0]=_frame.getWidth();
                        res[1]=_frame.getHeight();
                        shader.setUniform2fv("u_resolution",res);
                        
                        ofSetColor(255);
                        _frame.getTextureRef().draw(0,0,_frame.getWidth(),_frame.getHeight());
                    }
                    shader.end();
                }
                fbo.end();
                frameToSendOut = VideoFrame::newVideoFrame(fbo);
                paramFrameOut = frameToSendOut;
            }
        }
    }

}
