/*
 * VideoMixerGPU.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

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
	
    }

    //------------------------------------------------------------
    void FeedbackFilterNodeBased::setupNodeBased(){
        
        fps = -1;
        frame = VideoFrame();
        ofFbo fboAux;
        fboAux.allocate(640,480);
        frame = VideoFrame::newVideoFrame(fboAux);
        //ofAddListener(source->newFrameEvent,this,&GradientEdgesFilter::newVideoFrame);
        
        shader.load("shaders/feedback");
        
        if (true)
        {
            lastFrameRendered = ofxPm::VideoFrame();
        }
        
        parameters = new ofParameterGroup();
        parameters->setName("Feedback");
        parameters->add(paramScale.set("Scale",0.98,-1.0,2.0));
        parameters->add(paramMixAmmount.set("Mix Ammount",0.1,0.0,1.0));
        parameters->add(paramFrameIn.set("Frame Input", frame));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramFrameIn.addListener(this, &FeedbackFilterNodeBased::newVideoFrame);
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
            if (!isAllocated)
            {
                fbo.allocate(_frame.getWidth(), _frame.getHeight());
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
                        float* res = new float[2];
                        res[0]=_frame.getWidth();
                        res[1]=_frame.getHeight();
                        shader.setUniform2fv("u_resolution",res);
                        ofSetColor(255);
                        //    frame.getTextureRef().bind();
                        //	ofDrawRectangle(0,0,frame.getWidth(),frame.getHeight());
                        //            plane.draw();
                        //image.getTexture().draw(0, 0,640,480);
                        _frame.getTextureRef().draw(0,0,_frame.getWidth(),_frame.getHeight());
                    }
                    shader.end();
                }
                fbo.end();
                
                frame = VideoFrame::newVideoFrame(fbo);
                
                parameters->get("Frame Output").cast<ofxPm::VideoFrame>() = frame;
                
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
    }

}
