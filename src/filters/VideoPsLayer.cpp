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

        source = NULL;
        fps = -1;
        
        // allocate fbo where to draw
        if (!fbo.isAllocated())
        {
            fbo.allocate(640,480,GL_RGBA);
        }
                
        parameters->add(paramFrameIn.set("Frame Input 1", VideoFrame()));
        parameters->add(paramFrameIn2.set("Frame Input 2", VideoFrame()));
        parameters->add(paramOpacityBase.set("Opacity Base",1,0.0,1.0));
        parameters->add(paramOpacityTarget.set("Opacity Target",1,0.0,1.0));
        parameters->add(paramBlendMode.set("Blend Mode",0,0,25.0));
        parameters->add(paramFrameOut.set("Frame Output", VideoFrame()));
                
        paramFrameIn2.addListener(this, &VideoPsLayer::newVideoFrame);
        
        setFrameResolution(640, 480);

    }
    //------------------------------------------------------------
    void VideoPsLayer::update(ofEventArgs &e)
    {
        if(fboHasToBeAllocated != glm::vec2(-1, -1))
        {
            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
            fboHasToBeAllocated = glm::vec2(-1, -1);
        }
        
//        if(paramFrameIn.get()!=nullptr && paramFrameIn2.get()!=NULL)
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
    void VideoPsLayer::newVideoFrame(VideoFrame & _frame)
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
                
                frame = VideoFrame::newVideoFrame(fbo);
            }
        }
        paramFrameOut = frame;
    }
    
    //--------------------------------------------------------
    void VideoPsLayer::setFrameResolution(int _w, int _h)
    {
        cout << "VideoPsLayer :: Resetting frame resolution to " << _w << " , " << _h << endl;
        mixer.setup(_w,_h);
    }
}
