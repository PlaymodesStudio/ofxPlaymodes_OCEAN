
#ifndef VIDEORENDERERWINDOWNODEBASED_H_
#define VIDEORENDERERWINDOWNODEBASED_H_

#include "VideoSink.h"
#include "ofxOceanodeNodeModelExternalWindow.h"


namespace ofxPm{

class VideoRendererWindowNodeBased: public VideoSink, public ofxOceanodeNodeModelExternalWindow{

public:
	VideoRendererWindowNodeBased();
	~VideoRendererWindowNodeBased();

	void setup();
    void draw(int x,int y,int w,int h);
//    void draw(int x,int y);
//    void draw();
//    void draw(ofEventArgs &e);
    virtual void newVideoFrame(VideoFrame & frame);
    void        showMyExternalWindow(bool b);

    ofTexture getLastFrameTexture();
    
    ofParameter<float>          paramOpacity;

    void presetHasLoaded() override {
        parameters->getBool("Show Window") = true;
    }


private:
    VideoFrame  vFrame;
    
    void drawInExternalWindow(ofEventArgs &e) override;
    void keyPressed(ofKeyEventArgs &a) override;
    void mousePressed(ofMouseEventArgs &a) override;

};
}

#endif /* VIDEORENDERERWINDOWNODEBASED_H_ */
