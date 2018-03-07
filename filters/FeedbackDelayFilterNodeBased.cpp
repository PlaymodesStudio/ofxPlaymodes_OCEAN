/*
 * VideoMixerGPU.cpp
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#include "FeedbackDelayFilterNodeBased.h"
#include "parametersControl.h"


namespace ofxPm{
    //------------------------------------------------------------
    FeedbackDelayFilterNodeBased::FeedbackDelayFilterNodeBased()
    {
    }

    //------------------------------------------------------------
    FeedbackDelayFilterNodeBased::~FeedbackDelayFilterNodeBased()
    {
    }

    //------------------------------------------------------------
    void FeedbackDelayFilterNodeBased::setup(VideoSource & _source)
    {
//        source = &_source;
//        fps = _source.getFps();
//        isNodeBased = false;
//        
//        frame = VideoFrame::newVideoFrame(source->getNextVideoFrame());
//        ofAddListener(source->newFrameEvent,this,&FeedbackDelayFilterNodeBased::newVideoFrame);
//        //shader.setupShaderFromSource(GL_FRAGMENT_SHADER,fragmentTestSrc);
//        shader.load("shaders/gradient");
//        
//        //shader.linkProgram();
//        // allocate fbo where to draw
//        if(source->getWidth()>0) fbo.allocate(source->getWidth(),source->getHeight(),GL_RGBA);
//        else
//        {
//            cout << "VideoTestShaderFX !! WARNING : Fbo setup with default size !!" << endl;
//            fbo.allocate(frame.getWidth(),frame.getHeight(),GL_RGBA);
//        }
//        //    plane.set(source->getWidth(),source->getHeight(), 10, 10);
//        //    plane.mapTexCoords(0, 0,source->getWidth(),source->getHeight());
//        
//        cout <<"FX Source W : "  << source->getWidth() << " H : " << source->getHeight() << endl;
        

    }

    //------------------------------------------------------------
    void FeedbackDelayFilterNodeBased::setupNodeBased(){
        
        
        source = NULL;
        fps = -1;
        isNodeBased = true;
        
        frame = VideoFrame();
        ofFbo fboAux;
        fboAux.allocate(640,480);
        frame = VideoFrame::newVideoFrame(fboAux);
        //ofAddListener(source->newFrameEvent,this,&GradientEdgesFilter::newVideoFrame);
        
        shader.load("shaders/feedbackDelay");
        
        if (true)
        {
            lastFrameRendered = ofxPm::VideoFrame();
        }
        
        // param nodes
                
        parameters = new ofParameterGroup();
        parameters->setName("Feedback");
        parameters->add(paramVideoBufferInput.set("Buffer Input", nullptr));
        parameters->add(paramDelayMs.set("Delay Ms",0.0,0.0,4000.0));
        parameters->add(paramScale.set("Scale",0.98,-1.0,2.0));
        parameters->add(paramMixAmmount.set("Mix Ammount",0.1,0.0,1.0));
        parameters->add(paramDryWet.set("Dry Wet",0.0,0.0,1.0));
//        parameters->add(paramFrameIn.set("Frame Input", frame));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        
        parametersControl::getInstance().createGuiFromParams(parameters,ofColor::cyan);
        
        paramVideoBufferInput.addListener(this, &FeedbackDelayFilterNodeBased::setVideoBuffer);
        
//        paramGradientWidth.addListener(this, &FeedbackFilterNodeBased::setGradientWidth);
//        paramGradientXorY.addListener(this, &FeedbackFilterNodeBased::setGradientXorY);
        
        //paramFrameIn.addListener(this, &FeedbackDelayFilterNodeBased::newVideoFrame);
    }

VideoFrame FeedbackDelayFilterNodeBased::getNextVideoFrame()
{
    
//    if(source->getNextVideoFrame()!=NULL)
//    {
//        return source->getNextVideoFrame();
//    }
//    cout << " fx : getNextVF"  << endl;
    return frame;

}

    void FeedbackDelayFilterNodeBased::newVideoFrame(VideoFrame & _frame)
    {
//	//front = VideoFrame::newVideoFrame(frame);
//
//    VideoFrame f = source->getNextVideoFrame() ;
//	if(f!=NULL)
//    {
//        frame = f;
//    }
//
//
    if (!fbo.isAllocated())
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
                ofxPm::VideoFrame vf;
                ofxPm::VideoFrame vfPresent;

                videoHeaderDelay.setDelayMs(paramDelayMs);
                vf = VideoFrame();
                vf = videoHeaderDelay.getNextVideoFrame();

                videoHeaderDelay.setDelayMs(0.0);
                vfPresent = VideoFrame();
                vfPresent = videoHeaderDelay.getNextVideoFrame();

                if(!vf.isNull())
                {
                    shader.setUniformTexture("tex0",vf.getTextureRef(),0);
                    shader.setUniformTexture("tex1",frame.getTextureRef(),1);
                    shader.setUniformTexture("tex2",vfPresent.getTextureRef(),2);
                    
                    shader.setUniform1f("u_scale",paramScale);
                    shader.setUniform1f("u_mixAmmount",paramMixAmmount);
                    shader.setUniform1f("u_dryWet",paramDryWet);
                    
                    float* res = new float[2];
                    res[0]=frame.getWidth();
                    res[1]=frame.getHeight();
                    shader.setUniform2fv("u_resolution",res);
                    ofSetColor(255);
                    //    frame.getTextureRef().bind();
                    //	ofDrawRectangle(0,0,frame.getWidth(),frame.getHeight());
                    //            plane.draw();
                    //image.getTexture().draw(0, 0,640,480);
                    vf.getTextureRef().draw(0,0,vf.getWidth(),vf.getHeight());
                }
            }
            shader.end();
        }
        fbo.end();
        
        frame = VideoFrame::newVideoFrame(fbo);
        
        
        if(!isNodeBased) ofNotifyEvent(newFrameEvent,frame);
        else parameters->get("Frame Output").cast<ofxPm::VideoFrame>() = frame;
        
    }
    
    
}

    //-----------------------------------------
    void FeedbackDelayFilterNodeBased::setVideoBuffer(ofxPm::VideoBuffer* &_videoBuffer)
    {
        cout << "FeedBackDelayFilter :: setting Video Buffer ..." << endl;

//        parameters->get("Buffer Input").cast<ofxPm::VideoBuffer*>() = _videoBuffer;
        
//        paramVideoBufferInput = _videoBuffer;
        
        // allocate fbo where to draw
        if (fbo.getWidth()<=0)
        {
            // setup FBO
            int resX = paramVideoBufferInput.get()->getWidth();
            int resY = paramVideoBufferInput.get()->getHeight();
            fbo.allocate(resX,resY,GL_RGBA);
            
            // setup Headers
            videoHeaderDelay.setup(*paramVideoBufferInput.get());
            videoHeaderDelay.setDelayMs(0);
            
        }
        
        ofxPm::VideoFrame vf;
        newVideoFrame(vf);
    
    }

}
