

#ifndef VIDEOTRIORENDERERNODEBASED_H_
#define VIDEOTRIORENDERERNODEBASED_H_

#include "VideoSink.h"
#include "EdgeBlendNodeBased.h"
#include "ofxOceanodeNodeModelExternalWindow.h"


namespace ofxPm{
    
    class VideoTrioRendererNodeBased: public VideoSink, public ofxOceanodeNodeModelExternalWindow{
        
    public:
        VideoTrioRendererNodeBased();
        ~VideoTrioRendererNodeBased();
        
        void setup ();
        void draw(int x,int y,int w,int h);
        void newVideoFrame(VideoFrame & frame);
        void newVideoFrame2(VideoFrame & frame);

        ofTexture   getLastFrameTexture();
        void        showMyExternalWindow(bool b);
        
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

        ofImage                             backImage;
        
        void presetHasLoaded() override {
            parameters->getBool("Show Window") = true;
        }
        
    private:
        VideoFrame  vFrame;
        VideoFrame  vFrame2;

        vector<vector<int>> flips;
        vector<vector<int>> layouts;
        
        void changedFlipMode(int &m);
        
        void drawInExternalWindow(ofEventArgs &e) override;
        void keyPressed(ofKeyEventArgs &a) override;
        void mousePressed(ofMouseEventArgs &a) override;

    };
}

#endif /* VIDEOTRIORENDERERNODEBASED_H_ */


