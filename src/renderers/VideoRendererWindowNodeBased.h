
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

    ofTexture getLastFrameTexture();
    
    ofParameter<float>          paramOpacity;
        

private:
    VideoFrame  vFrame;
    
    void drawInExternalWindow(ofEventArgs &e) override;
    void keyPressed(ofKeyEventArgs &a) override;
    void mousePressed(ofMouseEventArgs &a) override;

};
}

#endif /* VIDEORENDERERWINDOWNODEBASED_H_ */
