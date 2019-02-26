
#ifndef HALFTONEFX_H_
#define HALFTONEFX_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class HalfToneFilter: public VideoFilter,public ofxOceanodeNodeModel {
public:
	HalfToneFilter();
	virtual ~HalfToneFilter();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame();
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return fps;};
    

private:
    ofFbo           fbo;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
    glm::vec2       fboHasToBeAllocated;

    float fps;
    
    ofParameter<float>                  paramColorSteps;
    ofParameter<float>                  paramScale;
    ofParameter<float>                  paramTimeFactor;
    ofParameter<float>                  paramBlur;

};
}

#endif
