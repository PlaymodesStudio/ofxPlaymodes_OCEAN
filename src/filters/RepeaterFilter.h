

#ifndef REPEATERFILTER_H
#define REPEATERFILTER_H

#include "VideoFilter.h"
#include "VideoBufferNodeBased.h"
#include "VideoHeaderNodeBased.h"
//#include "ofxOceanodeNodeModelLocalPreset.h"
#include "basePhasor.h"

#include "ofMain.h"

namespace ofxPm{
        
class RepeaterFilter: public VideoFilter,public ofxOceanodeNodeModel
{
public:
    RepeaterFilter();
    virtual ~RepeaterFilter();
    
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
    ofParameter<int>                    paramRefreshLoopAt;
    ofParameter<float>                  paramSpeedBoost;

    ofxPm::VideoBufferNodeBased                 buffer;
    ofxPm::VideoHeaderNodeBased                 videoHeader;
    bool                                        restart;

    void                                loopTimeChanged(int& _i);
    void                                doLoopChanged(bool& _b);
    void                                doRestart();
    // when global BPM changes, it will call this function so I can know the global bpm
    void                                setBpm(float _bpm) override{
        myBPM=_bpm;
        int i;
        loopTimeChanged(i);
        _phasor.setBpm(_bpm);
    };
private:

    // rendering
    VideoFrame                          myFrame;
    double                              loopDurationMs;
    double                              loopStartedAtMs;
    double                              loopDurationMsWhenTriggered;
    double                              BPMfactor;
    float                               myBPM;
    bool                                oldDoLoop;
    
    basePhasor                          _phasor;
    
    void                                phasorCycleEvent();
    int                                 phasorNumCycles;
    
    ofEventListeners listeners;
};
}

#endif // REPEATERFILTER_H
