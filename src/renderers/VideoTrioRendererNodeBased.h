

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
        
        void setup();
        void draw(int x,int y,int w,int h);
        //    void draw(int x,int y);
        //    void draw();
        //    void draw(ofEventArgs &e);
        void newVideoFrame(VideoFrame & frame);
        void newVideoFrame2(VideoFrame & frame);
        void newVideoFrame3(VideoFrame & frame);

        ofTexture getLastFrameTexture();
        
        ofParameter<float>                  paramOpacity;
        ofParameter<float>                  paramScale;
        ofParameter<float>                  paramOverlap;
        ofParameter<bool>                   paramMinMaxBlend;
        ofParameter<ofxPm::VideoFrame>      paramFrameIn2;
        ofParameter<ofxPm::VideoFrame>      paramFrameIn3;
        ofParameter<float>                  paramGradientWidth;
        ofParameter<int>                    paramGradientXorY;

        ofxPm::EdgeBlendNodeBased           edgeBlend;
        
    private:
        VideoFrame  vFrame;
        VideoFrame  vFrame2;
        VideoFrame  vFrame3;

        void drawInExternalWindow(ofEventArgs &e) override;
        void keyPressed(ofKeyEventArgs &a) override;
        void mousePressed(ofMouseEventArgs &a) override;
        
    };
}

#endif /* VIDEOTRIORENDERERNODEBASED_H_ */


