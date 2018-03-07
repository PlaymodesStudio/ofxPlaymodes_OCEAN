
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
    virtual void newVideoFrame(VideoFrame & frame);

    ofTexture getLastFrameTexture();
    
    ofxOceanodeAbstractConnection* createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink) override;
    

private:
    VideoFrame  vFrame;
};
}

#endif /* VIDEORENDERERNODEBASED_H_ */
