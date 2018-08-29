/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef VIDEOGAMMASHADERFX_H_
#define VIDEOGAMMASHADERFX_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class GammaFilter: public VideoFilter,public ofxOceanodeNodeModel {
public:
	GammaFilter();
	virtual ~GammaFilter();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame(){ return VideoFrame(); };
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return fps;};
    void setMin(float &f){paramMin=f;};
    void setMax(float &f){paramMax=f;};
    

private:
    ofFbo           fbo;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
	//ofMutex swapBuffersMutex;
    glm::vec2       fboHasToBeAllocated;

    
    float fps;
    
    // FEATURE NODE
    ofParameter<float>                  paramMin;
    ofParameter<float>                  paramMax;
    ofParameter<float>                  paramGamma;
//    ofParameter<ofxPm::VideoFrame>      paramFrameIn;
//    ofParameter<ofxPm::VideoFrame>      paramFrameOut;

};
}

#endif /* VIDEOGAMMASHADERFX_H_ */
