/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef VIDEOTESTSHADERFX_H_
#define VIDEOTESTSHADERFX_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class LumaFilterNodeBased: public VideoFilter,public ofxOceanodeNodeModel {
public:
	LumaFilterNodeBased();
	virtual ~LumaFilterNodeBased();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame();
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return fps;};
    void setLumaThreshold(float &f){paramLumaThrshold=f;};
    void setLumaSmooth(float &f){paramLumaSmooth=f;};
    

private:
    ofFbo           fbo;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
	//ofMutex swapBuffersMutex;
    glm::vec2       fboHasToBeAllocated;

    float fps;
    
    // FEATURE NODE
    ofParameter<float>                  paramLumaThrshold;
    ofParameter<float>                  paramLumaSmooth;
//    ofParameter<ofxPm::VideoFrame>      paramFrameIn;
//    ofParameter<ofxPm::VideoFrame>      paramFrameOut;

};
}

#endif /* VIDEOTESTSHADERFX_H_ */
