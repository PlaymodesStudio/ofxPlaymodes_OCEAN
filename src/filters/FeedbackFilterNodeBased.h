/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef FEEEDBACK_FILTER_H
#define FEEEDBACK_FILTER_H

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class FeedbackFilterNodeBased: public VideoFilter,public ofxOceanodeNodeModel
    {
public:
	FeedbackFilterNodeBased();
	virtual ~FeedbackFilterNodeBased();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

        
    VideoFrame getNextVideoFrame();
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return fps;};

    
private:
	VideoFrame      frameToSendOut;
    ofFbo           fbo;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
	//ofMutex swapBuffersMutex;
    float fps;
    glm::vec2       fboHasToBeAllocated;

    
    ofParameter<float>                  paramScale;
    ofParameter<float>                  paramMixAmmount;
    ofParameter<float>                  paramOriginX;
    ofParameter<float>                  paramOriginY;

    ofEventListener listener;
};
}

#endif /* FEEEDBACK_FILTER_H */
