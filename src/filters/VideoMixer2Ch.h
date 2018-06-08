/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef VIDEOMIXER2CH_H_
#define VIDEOMIXER2CH_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class VideoMixer2Ch: public VideoFilter,public ofxOceanodeNodeModel {
public:
	VideoMixer2Ch();
	virtual ~VideoMixer2Ch();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame(){return VideoFrame();};
	void newVideoFrame(VideoFrame & frame);
    void newVideoFrame2(VideoFrame & frame);
    float getFps(){return source->getFps();};
    
private:
    
	VideoFrame      frame;
    ofFbo           fbo;
	VideoSource     *source;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
    glm::vec2       fboHasToBeAllocated;

    float fps;

    ofTexture       textureIn2;

    
    // FEATURE NODE
    ofParameter<float>                  paramCrossfade;
    ofParameter<int>                    paramMixMode;
    ofParameter<ofxPm::VideoFrame>      paramFrameIn2;
//    ofParameter<ofxPm::VideoFrame>      paramFrameIn;
//    ofParameter<ofxPm::VideoFrame>      paramFrameOut;

};
}

#endif /* VIDEOMIXER2CH_H_ */
