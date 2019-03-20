

#ifndef GATORFILTER_H
#define GATORFILTER_H

#include "VideoFilter.h"
#include "VideoBufferNodeBased.h"
#include "VideoHeaderNodeBased.h"
#include "basePhasor.h"

#include "ofMain.h"

namespace ofxPm{
    
    // TO DO
    // Resize b&w frames when videoframe size changes !!

    
class GatorFilter: public VideoFilter,public ofxOceanodeNodeModel
{
public:
    GatorFilter();
    virtual ~GatorFilter();
    
    void        setupNodeBased();
    VideoFrame  getNextVideoFrame() override;
    void        newVideoFrame(VideoFrame & _frame)override;
    float       getFps()override {return fps;};
    
protected:
    
    float                               fps;
    ofParameter<bool>                   paramBlackOrWhite;
    ofParameter<int>                    paramGateFreqBPM;
    ofParameter<void>                   paramDoRestart;
    ofParameter<bool>                   paramBypass;

    void                                freqChanged(int& _i);
    void                                restartChanged();
    // when global BPM changes, it will call this function so I can know the global bpm
    void                                setBpm(float _bpm) override{myBPM=_bpm;};

private:

    // rendering
    VideoFrame                          myFrame;
    VideoFrame                          whiteFrame;
    VideoFrame                          blackFrame;
    void                                setFrameResolution(int _w, int _h);
    
    float                               myBPM;
    
    basePhasor                          _phasor;
    void                                phasorCycleEvent();

    vector<float>                       beatMultDiv;
    bool                                doGate;
    glm::vec2                           frameResolution;
    
    ofEventListeners listeners;
};
}

#endif // GATORFILTER_H
