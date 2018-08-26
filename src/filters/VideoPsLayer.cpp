#include "VideoPsLayer.h"

namespace ofxPm{
    
    //--------------------------------------------------------------
    VideoPsLayer::VideoPsLayer():ofxOceanodeNodeModel("Ps Layer")
    {
        setupNodeBased();

    }

    //--------------------------------------------------------------
    VideoPsLayer::~VideoPsLayer()
    {
        fbo.clear();
    }

    //--------------------------------------------------------------
    void VideoPsLayer::setupNodeBased()
    {
        color = ofColor::yellow;
        fps = -1;
        
        parameters->add(paramFrameIn.set("Frame Input 1", VideoFrame()));
        parameters->add(paramFrameIn2.set("Frame Input 2", VideoFrame()));
        parameters->add(paramOpacityBase.set("Opacity Base",1,0.0,1.0));
        parameters->add(paramOpacityTarget.set("Opacity Target",1,0.0,1.0));
        parameters->add(paramBlendMode.set("Blend Mode",0,0,25.0));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
                
        paramFrameIn2.addListener(this, &VideoPsLayer::newVideoFrame);
        
    }
    //------------------------------------------------------------
    void VideoPsLayer::update(ofEventArgs &e)
    {
        VideoFrame vf1 = paramFrameIn.get();
        if(!vf1.isNullPtr())
        {
            mixer.begin();
            VideoFrame vf1 = paramFrameIn.get();
            
            ofSetColor(255,255,255,255);
            vf1.getTextureRef().draw(0,0,640,480);
            mixer.end();
        }
    }
    
    //--------------------------------------------------------------
    void VideoPsLayer::newVideoFrame(VideoFrame & _frame)
    {
        bool isAllocated = fbo.isAllocated();
        bool frameIsNull = _frame.isNull();
        
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
                    
                    {
                        
                        ofSetColor(255);
                        int w = _frame.getWidth();
                        int h = _frame.getHeight();
                        ofSetColor(255);
                        mixer.setOpacityBase(paramOpacityBase);
                        mixer.setOpacityTarget(paramOpacityTarget);
                        mixer.draw(_frame.getTextureRef(), paramBlendMode);
                        
                        //_frame.getTextureRef().draw(0,0,6);
                    }
                    
                }
                fbo.end();
                
                paramFrameOut = VideoFrame::newVideoFrame(fbo);
            }
        }
    }
    
    //--------------------------------------------------------
    void VideoPsLayer::setFrameResolution(int _w, int _h)
    {
        cout << "VideoPsLayer :: Resetting frame resolution to " << _w << " , " << _h << endl;
        mixer.setup(_w,_h);
    }
}
