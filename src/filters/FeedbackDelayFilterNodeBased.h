///*
// * VideoMixerGPU.h
// *
// *  Created on: Jan 18, 2013
// *      Author: arturo
// */
//
//#ifndef FEEEDBACKDELAY_FILTER_H
//#define FEEEDBACKDELAY_FILTER_H
//
//#include "VideoFilter.h"
//#include "VideoBuffer.h"
//#include "VideoHeader.h"
//
//namespace ofxPm{
//    
//class FeedbackDelayFilterNodeBased: public VideoFilter {
//public:
//    FeedbackDelayFilterNodeBased();
//    virtual ~FeedbackDelayFilterNodeBased();
//
//    void setup(VideoSource & source1);
//    void setupNodeBased();
//    
//    VideoFrame getNextVideoFrame();
//    void newVideoFrame(VideoFrame & frame);
//    float getFps(){return source->getFps();};
//    void    setVideoBuffer(ofxPm::VideoBuffer* &videoBuffer);
//
//
//private:
//    VideoFrame      frame;
//    ofFbo           fbo;
//    VideoSource     *source;
//    ofShader        shader;
//    bool            newFrame;
//    ofPlanePrimitive plane;
//    //ofMutex swapBuffersMutex;
//
//    float fps;
//    
//    //    float gradientWidth;
//    //    int gradientXorY;
//    // FEATURE NODE
//    ofParameterGroup*                   parameters;
//    ofParameter<float>                  paramScale;
//    ofParameter<float>                  paramMixAmmount;
//    ofParameter<float>                  paramDryWet;
//    ofParameter<ofxPm::VideoBuffer*>    paramVideoBufferInput;
//    ofParameter<float>                  paramDelayMs;
//    
//    bool                                isNodeBased;
//
//    VideoFrame                          lastFrameRendered;
//    VideoHeader                         videoHeaderDelay;
//};
//}
//
//#endif /* FEEEDBACKDELAY_FILTER_H */

