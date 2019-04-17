//
//  MirrorRenderer.h
//  VJYourself_OCEAN
//
//  Created by Playmodes Studio on 05/04/2019.
//

#ifndef MirrorRenderer_h
#define MirrorRenderer_h

#include "ofxOceanodeNodeModel.h"
#include "VideoFilter.h"

namespace ofxPm{
    
    class MirrorRenderer :  public VideoFilter, public ofxOceanodeNodeModel{
    public:
        MirrorRenderer();
        ~MirrorRenderer(){};
        
        void setup() override;
        VideoFrame getNextVideoFrame() override{};
        float getFps() override{};
        
    private:
        ofParameter<int>    mirrorSize;
        ofParameter<float>  spacing;
        ofParameter<float>  scale;
        ofParameter<bool> mirrorInvert;
        
        ofEventListeners listeners;
        
        ofFbo fbo;
    };
    
}

#endif /* MirrorRenderer_h */
