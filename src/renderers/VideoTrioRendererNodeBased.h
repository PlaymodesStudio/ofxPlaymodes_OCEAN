

#ifndef VIDEOTRIORENDERERNODEBASED_H_
#define VIDEOTRIORENDERERNODEBASED_H_

#include "VideoSink.h"
#include "EdgeBlendNodeBased.h"
#include "ofxOceanodeNodeModelExternalWindow.h"


namespace ofxPm{
    
    class VideoTrioRendererNodeBased: public VideoFilter, public ofxOceanodeNodeModel{
        
    public:
        VideoTrioRendererNodeBased();
        ~VideoTrioRendererNodeBased();
        
        void setup ();
        void draw(int x,int y,int w,int h);
        void drawIntoFbo(int x,int y,int w,int h);
        void newVideoFrame(VideoFrame & frame);
        void newVideoFrame2(VideoFrame & frame);
        VideoFrame getNextVideoFrame(){return paramFrameOut;};
        float getFps(){return fps;};

        ofTexture   getLastFrameTexture();
        
        ofParameter<float>                  paramOpacity;
        ofParameter<float>                  paramScale;
        ofParameter<float>                  paramOverlap;
        ofParameter<bool>                   paramMinMaxBlend;
        ofParameter<ofxPm::VideoFrame>      paramFrameIn2;
        ofParameter<float>                  paramGradientWidth;
        ofParameter<int>                    paramGradientXorY;
        ofParameter<int>                    paramFlipMode;
        ofParameter<int>                    paramLayout;

        VideoFrame* frameRefCentre;
        VideoFrame* frameRefDreta;
        VideoFrame* frameRefEsquerra;
        
        void presetHasLoaded() override {}
        
    private:
        VideoFrame  vFrame;
        VideoFrame  vFrame2;
        ofFbo       fbo;
        float       fps;

        vector<vector<int>> flips;
        vector<vector<int>> layouts;

        void changedFlipMode(int &m);

        ofEventListeners listeners;
    };
}

#endif /* VIDEOTRIORENDERERNODEBASED_H_ */


