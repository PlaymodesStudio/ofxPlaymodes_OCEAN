
#ifndef EDGEBLENDNODEBASED_H_
#define EDGEBLENDNODEBASED_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class EdgeBlendNodeBased: public VideoFilter,public ofxOceanodeNodeModel {
public:
	EdgeBlendNodeBased();
	virtual ~EdgeBlendNodeBased();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame(){return paramFrameOut;};
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return source->getFps();};
    void setGradientWidth(float &_f){paramGradientWidth=_f;};
    void setGradientXorY(int &_i){paramGradientXorY=_i;};


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
    ofParameter<float>                  paramGradientWidth;
    ofParameter<int>                    paramGradientXorY;
};
}

#endif /* EDGEBLENDNODEBASED_H_ */
