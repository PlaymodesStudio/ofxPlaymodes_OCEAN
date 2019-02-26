
#ifndef INPUTPROCESSING_H_
#define INPUTPROCESSING_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
class inputProcessingFilter: public VideoFilter,public ofxOceanodeNodeModel {
public:
	inputProcessingFilter();
	virtual ~inputProcessingFilter();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame();
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return source->getFps();};
    void setGradientWidth(float &_f){paramGradientWidth=_f;};
    void setGradientXorY(int &_i){paramGradientXorY=_i;};
    void setLumaThreshold(float &f){paramLumaThrshold=f;};
    void setLumaSmooth(float &f){paramLumaSmooth=f;};



private:
	VideoFrame      frame;
    ofFbo           fbo;
	VideoSource     *source;
	ofShader        shader;
	bool            newFrame;
    ofPlanePrimitive plane;
    
    float fps;
    
    // FEATURE NODE
    ofParameter<float>                  paramGradientWidth;
    ofParameter<int>                    paramGradientXorY;
    ofParameter<float>                  paramLumaThrshold;
    ofParameter<float>                  paramLumaSmooth;
    ofParameter<float>                  paramMin;
    ofParameter<float>                  paramMax;
    ofParameter<float>                  paramGamma;


};
}

#endif /* INPUTPROCESSING_H_ */
