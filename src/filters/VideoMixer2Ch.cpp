#include "VideoMixer2Ch.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    VideoMixer2Ch::VideoMixer2Ch():ofxOceanodeNodeModel("Mixer 2Ch")
    {
        setupNodeBased();
    }

    //--------------------------------------------------------------
    VideoMixer2Ch::~VideoMixer2Ch()
    {
        fbo.clear();
        shader.unload();
    }

    //--------------------------------------------------------------
    void VideoMixer2Ch::setupNodeBased()
    {
        color = ofColor::yellow;

        source = NULL;
        fps = -1;
        
        string shaderName = "shaders/mixer2ch";
        shader.load(shaderName);
        cout << "Mixer::Loading Shader : " << shaderName << endl;
        // allocate fbo where to draw
        if (fbo.isAllocated())
        {
            fbo.allocate(source->getWidth(),source->getHeight(),GL_RGBA);
        }

        parameters->add(paramFrameIn.set("Frame Input 1", frame));
        parameters->add(paramFrameIn2.set("Frame Input 2", frame));
        parameters->add(paramCrossfade.set("Crossfade",0.0,0.0,1.0));
        parameters->add(paramMixMode.set("Mix Mode",0,0,2));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
//        paramLumaThrshold.addListener(this, &VideoMixer2Ch::setLumaThreshold);
        
        paramFrameIn.addListener(this, &VideoMixer2Ch::newVideoFrame);
        paramFrameIn2.addListener(this, &VideoMixer2Ch::newVideoFrame2);
        
        textureIn2.allocate(640,480,GL_RGB);

    }
    //------------------------------------------------------------
    void VideoMixer2Ch::update(ofEventArgs &e)
    {
        if(fboHasToBeAllocated != glm::vec2(-1, -1))
        {
            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
            fboHasToBeAllocated = glm::vec2(-1, -1);
        }
    }
    
    //--------------------------------------------------------------
    //VideoFrame VideoMixer2Ch::getNextVideoFrame()
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
    void VideoMixer2Ch::newVideoFrame2(VideoFrame & _frame)
    {
        if(!_frame.isNull())
        {
            textureIn2 = _frame.getTextureRef();
        }
        else textureIn2.clear();
        
    }
    //--------------------------------------------------------------
    void VideoMixer2Ch::newVideoFrame(VideoFrame & _frame)
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
                        shader.setUniformTexture("tex1",textureIn2,12);
                        shader.setUniform1f("u_crossfade",paramCrossfade);
                        shader.setUniform1i("u_mixMode", paramMixMode);
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
