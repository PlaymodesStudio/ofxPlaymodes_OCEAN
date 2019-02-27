/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef VIDEOSPLITER_H_
#define VIDEOSPLITER_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class VideoSpliter: public VideoFilter,public ofxOceanodeNodeModel {
public:
	VideoSpliter();
	virtual ~VideoSpliter();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame(){ return VideoFrame(); };
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return fps;};
    
private:
    
    float fps;
    ofParameter<ofxPm::VideoFrame>      paramFrameOut2;

    ofEventListener listener;
};
}

#endif /* VIDEOSPLITER_H_ */
