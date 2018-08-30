/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef COLORCORRECTFILTER_H_
#define COLORCORRECTFILTER_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class ColorCorrectFilter: public VideoFilter,public ofxOceanodeNodeModel {
public:
	ColorCorrectFilter();
	virtual ~ColorCorrectFilter();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
	VideoFrame getNextVideoFrame() { return VideoFrame(); };
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return fps;};

private:
    ofFbo           fbo;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
	//ofMutex swapBuffersMutex;
    glm::vec2       fboHasToBeAllocated;

    
    float fps;
    
    // FEATURE NODE
    ofParameter<float>                  paramRed;
    ofParameter<float>                  paramGreen;
    ofParameter<float>                  paramBlue;

};
}

#endif /* COLORCORRECTFILTER_H_ */
