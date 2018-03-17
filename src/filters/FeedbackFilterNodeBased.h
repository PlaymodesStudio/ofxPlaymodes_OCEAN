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
    
    VideoFrame getNextVideoFrame();
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return fps;};

    
private:
	VideoFrame      frame;
    ofFbo           fbo;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
	//ofMutex swapBuffersMutex;
    float fps;
    VideoFrame      lastFrameRendered;
    
    ofxOceanodeAbstractConnection* createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink) override;

    ofParameter<float>                  paramScale;
    ofParameter<float>                  paramMixAmmount;
    ofParameter<float>                  paramOriginX;
    ofParameter<float>                  paramOriginY;

};
}

#endif /* FEEEDBACK_FILTER_H */
