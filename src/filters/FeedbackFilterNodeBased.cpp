
#include "FeedbackFilterNodeBased.h"
#include "ofxOceanodeContainer.h"

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
        
        fps = -1;
        
        //frame = VideoFrame();
        //ofFbo fboAux;
        //fboAux.allocate(640,480);
        //frame = VideoFrame::newVideoFrame(fboAux);
        
        shader.load("shaders/feedback");
        
        if (true)
        {
            lastFrameRendered = ofxPm::VideoFrame();
        }
        
        parameters = new ofParameterGroup();
        parameters->setName("Feedback");
        parameters->add(paramFrameIn.set("Frame Input", frame));
        parameters->add(paramScale.set("Scale",0.98,-1.0,2.0));
        parameters->add(paramMixAmmount.set("Mix Ammount",0.1,0.0,1.0));
        parameters->add(paramOriginX.set("OriginX",0.5,-1.0,1.0));
        parameters->add(paramOriginY.set("OriginY",0.5,-1.0,1.0));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramFrameIn.addListener(this, &FeedbackFilterNodeBased::newVideoFrame);
    }

    //------------------------------------------------------------
    void FeedbackFilterNodeBased::update(ofEventArgs &e)
    {
        if(fboHasToBeAllocated != glm::vec2(-1, -1))
        {
            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
            fboHasToBeAllocated = glm::vec2(-1, -1);
        }
    }

    //------------------------------------------------------------
    VideoFrame FeedbackFilterNodeBased::getNextVideoFrame()
    {
        return frame;

    }

    //------------------------------------------------------------
    void FeedbackFilterNodeBased::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        
        
        if(!frameIsNull)
        {
            // this is for the start of the module ... we need to have something on "frame" as the lastframe.
            if(frame.isNull())
            {
                frame = _frame;
            }

            // 
            if (!isAllocated || _frame.getWidth() != fbo.getWidth() || _frame.getHeight() != fbo.getHeight())
            {
                fboHasToBeAllocated = glm::vec2(_frame.getWidth(), _frame.getHeight());
            }
            else
            {
                fbo.begin();
                {
                    ofClear(0, 0, 0, 255);
                    shader.begin();
                    {
                        shader.setUniformTexture("tex0",_frame.getTextureRef(),0);
                        shader.setUniformTexture("tex1",frame.getTextureRef(),1);
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
                frame = VideoFrame::newVideoFrame(fbo);
                paramFrameOut = frame;
            }
        }
    }
    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* FeedbackFilterNodeBased::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoFrame>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoFrame>(), sink.cast<ofxPm::VideoFrame>());
        }
        return nullptr;
    }

}
