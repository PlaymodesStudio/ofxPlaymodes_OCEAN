
#ifndef VIDEORENDERERNODEBASED_H_
#define VIDEORENDERERNODEBASED_H_

#include "VideoSink.h"
#include "ofxOceanodeNodeModel.h"


namespace ofxPm{

class VideoRendererNodeBased: public VideoSink, public ofxOceanodeNodeModel{

public:
	VideoRendererNodeBased();
	~VideoRendererNodeBased();

	void setup();
    void draw(int x,int y,int w,int h);
    void draw(int x,int y);
    void draw();
    void draw(ofEventArgs &e);
    void newVideoFrame(VideoFrame & frame);

    ofTexture getLastFrameTexture();
    
    ofParameter<int>            paramResolutionX;
    ofParameter<int>            paramResolutionY;
    ofParameter<int>            paramPositionX;
    ofParameter<int>            paramPositionY;
    ofParameter<float>          paramOpacity;
        

private:
    VideoFrame  vFrame;
};
}

#endif /* VIDEORENDERERNODEBASED_H_ */
