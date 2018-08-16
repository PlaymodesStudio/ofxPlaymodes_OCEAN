
#ifndef KALEIDOSCOPENODEBASED_H_
#define KALEIDOSCOPENODEBASED_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class kaleidoscopeNodeBased: public VideoFilter,public ofxOceanodeNodeModel {
public:
	kaleidoscopeNodeBased();
	virtual ~kaleidoscopeNodeBased();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame(){return paramFrameOut;};
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return source->getFps();};


private:
	VideoFrame      frame;
    ofFbo           fbo;
	VideoSource     *source;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
    glm::vec2       fboHasToBeAllocated;

    float lumaThreshold;
    float lumaSmooth;
    float fps;
    
    // FEATURE NODE
    ofParameter<int>                    paramSides;
    ofParameter<float>                  paramAngle;
    ofParameter<float>                  paramSlideX;
    ofParameter<float>                  paramSlideY;
};
}

#endif /* KALEIDOSCOPENODEBASED_H_ */
