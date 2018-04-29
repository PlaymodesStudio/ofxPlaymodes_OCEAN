/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef LOOPERFILTER_H
#define LOOPERFILTER_H

#include "VideoFilter.h"
#include "VideoBufferNodeBased.h"
#include "VideoHeaderNodeBased.h"
#include "ofxOceanodeNodeModelLocalPreset.h"
#include "basePhasor.h"

#include "ofMain.h"

namespace ofxPm{
        
class LooperFilter: public VideoFilter,public ofxOceanodeNodeModelLocalPreset
{
public:
    LooperFilter();
    virtual ~LooperFilter();
    
    void        setupNodeBased();
    VideoFrame  getNextVideoFrame();
    void        newVideoFrame(VideoFrame & _frame);
    float       getFps(){return fps;};
    
protected:
    
    float                               fps;
    ofParameter<bool>                   paramDoLoop;
    ofParameter<int>                    paramCapturedTimeBeatDiv;
    ofParameter<int>                    paramCapturedTimeBeatMult;
    ofParameter<float>                  paramGatePct;
    ofParameter<void>                   paramRestart;
    ofParameter<int>                    paramOffsetMs;

    ofxPm::VideoBufferNodeBased                 buffer;
    ofxPm::VideoHeaderNodeBased                 videoHeader;
    bool                                        restart;

    void                                loopTimeChanged(int& _i);
    void                                doLoopChanged(bool& _b);
    void                                doRestart();

private:

    // rendering
    VideoFrame                          myFrame;
    double                              loopDurationMs;
    double                              loopStartedAtMs;
    double                              loopDurationMsWhenTriggered;
    double                              BPMfactor;
    
    basePhasor                          _phasor;
    
    
};
}

#endif // LOOPERFILTER_H
