
#ifndef FRAMERATE_CALCULATOR_H
#define FRAMERATE_CALCULATOR_H

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    

    class FrameInspector: public VideoFilter,public ofxOceanodeNodeModel
    {
    public:
        
        FrameInspector();
        virtual ~FrameInspector();
        void setupNodeBased();
        
        VideoFrame  getNextVideoFrame() override;
        void        newVideoFrame(VideoFrame & frame) override;
        float       getFps(){return fps;};

    private:
        float                               fps;
        
        VideoFrame      frame;
        VideoFrame      lastFrameRendered;

        ofParameter<float>                  paramFPSNow;
        ofParameter<int>                    paramFPSAccum;
        ofParameter<int>                    paramFrameWidth;
        ofParameter<int>                    paramFrameHeight;
        float                               lastTimeMillis;
        deque<float>                        lastTimesVector;
        
        float                               getVectorMean();
        int                                 recordedTimes;

        ofEventListener listener;
};
}

#endif
