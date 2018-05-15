

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
    ofParameter<bool>                   paramDoRec;
    ofParameter<int>                    paramCapturedTimeBeatDiv;
    ofParameter<int>                    paramCapturedTimeBeatMult;
    ofParameter<float>                  paramGatePct;
    ofParameter<void>                   paramRestart;
    ofParameter<int>                    paramRefreshLoopAt;
    ofParameter<float>                  paramSpeedBoost;
    ofParameter<float>                  paramOffsetMs;
    

    ofxPm::VideoBufferNodeBased                 buffer;
    ofxPm::VideoHeaderNodeBased                 videoHeader;
    bool                                        restart;

    void                                loopTimeChanged(int& _i);
    void                                doLoopChanged(bool& _b);
    void                                doRecChanged(bool& _b);
    void                                doRestart();
    // when global BPM changes, it will call this function so I can know the global bpm
    void                                setBpm(float _bpm) override{myBPM=_bpm;int i;loopTimeChanged(i);};


private:

    // rendering
    VideoFrame                          myFrame;
    double                              loopDurationMs;
    double                              BPMfactor;
    float                               myBPM;
    
    basePhasor                          _phasor;
    
    void                                phasorCycleEvent();
    int                                 phasorNumCycles;
    
    
};
}

#endif // LOOPERFILTER_H
