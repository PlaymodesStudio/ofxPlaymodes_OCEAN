

#ifndef VIDEOPSLAYERFX_H_
#define VIDEOPSLAYERFX_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"
#include "ofxPSBlend.h"

namespace ofxPm{
    
class VideoPsLayer: public VideoFilter,public ofxOceanodeNodeModel {
public:
	VideoPsLayer();
	virtual ~VideoPsLayer();

    void setupNodeBased();
    void update(ofEventArgs &e) override;

    
    VideoFrame getNextVideoFrame(){};
	void newVideoFrame(VideoFrame & frame);
    float getFps(){return source->getFps();};
    

private:

    ofFbo           fbo;
	VideoSource     *source;
    ofxPSBlend      mixer;
	bool            newFrame;
    ofPlanePrimitive plane;
	//ofMutex swapBuffersMutex;
    
    float fps;
    
    glm::vec2                           frameResolution;
    void                                setFrameResolution(int _w, int _h);

    // FEATURE NODE
    ofParameter<VideoFrame>             paramFrameIn2;
    ofParameter<float>                  paramOpacityBase;
    ofParameter<float>                  paramOpacityTarget;
    ofParameter<int>                    paramBlendMode;
    
};
}

#endif /* VIDEOPSLAYERFX_H_ */
