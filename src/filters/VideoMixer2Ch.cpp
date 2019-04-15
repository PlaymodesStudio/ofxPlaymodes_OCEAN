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
        fps = -1;
        
        string shaderName = "shaders/mixer2ch";
        shader.load(shaderName);
        cout << "Mixer::Loading Shader : " << shaderName << endl;

        parameters->add(paramFrameIn.set("Frame Input 1", VideoFrame()));
        parameters->add(paramFrameIn2.set("Frame Input 2", VideoFrame()));
        parameters->add(paramCrossfade.set("Crossfade",0.0,0.0,1.0));
        parameters->add(paramMixMode.set("Mix Mode",0,0,2));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
        
        
        listeners.push(paramFrameIn.newListener(this, &VideoMixer2Ch::newVideoFrame));
        listeners.push(paramFrameIn2.newListener(this, &VideoMixer2Ch::newVideoFrame2));
        
        textureIn2.allocate(1,1,GL_RGB);

    }
    //------------------------------------------------------------
    void VideoMixer2Ch::update(ofEventArgs &e)
    {
    }
    
    //--------------------------------------------------------------
    void VideoMixer2Ch::newVideoFrame2(VideoFrame & _frame)
    {
        if(!_frame.isNull())
        {
            int fw,fh;
            fw = _frame.getWidth();
            fh = _frame.getHeight();
            if(fw!=textureIn2.getWidth() || fh!=textureIn2.getHeight())
            {
                textureIn2.allocate(fw,fh,GL_RGB);
            }
            textureIn2 = _frame.getTextureRef();
        }
        else textureIn2.clear();
        
    }
    //--------------------------------------------------------------
    void VideoMixer2Ch::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        float thereIstextureIn2=0.0;
        
        if(textureIn2.getWidth()>1)
        {
            cout << textureIn2.getWidth() << endl;
            thereIstextureIn2=1.0;
        }
        
        if(!frameIsNull)
        {
            int w = _frame.getWidth();
            int h = _frame.getHeight();
            
            if (!isAllocated || _frame.getWidth() != fbo.getWidth() || _frame.getHeight() != fbo.getHeight())
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
                        shader.setUniformTexture("tex1",textureIn2,12);
                        shader.setUniform1f("u_crossfade",paramCrossfade);
                        shader.setUniform1i("u_mixMode", paramMixMode);
                        shader.setUniform1f("u_isChannel2", thereIstextureIn2);
                        ofSetColor(255);
                        int w = _frame.getWidth();
                        int h = _frame.getHeight();
                        
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
