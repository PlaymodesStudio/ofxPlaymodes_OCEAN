
#ifndef FRAMERATE_CALCULATOR_H
#define FRAMERATE_CALCULATOR_H

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    

    class FrameRateCalculator: public VideoFilter,public ofxOceanodeNodeModel
    {
    public:
        
        FrameRateCalculator();
        virtual ~FrameRateCalculator();
        void setupNodeBased();
        
        VideoFrame  getNextVideoFrame() override;
        void        newVideoFrame(VideoFrame & frame) override;
        float       getFps(){return fps;};

    private:
        float                               fps;
        
        VideoFrame      frame;
        VideoFrame      lastFrameRendered;

        ofxOceanodeAbstractConnection* createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink) override;

        ofParameter<float>                  paramFPSNow;
        ofParameter<int>                  paramFPSAccum;
        float                               lastTimeMillis;
        deque<float>                        lastTimesVector;
        
        float                               getVectorMean();
        int                                 recordedTimes;

};
}

#endif
