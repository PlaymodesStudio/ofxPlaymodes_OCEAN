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
#include "ofxOceanodeNodeModel.h"
#include "defaultNodes.h"

#include "ofMain.h"

namespace ofxPm{
        
class LooperFilter: public VideoFilter,public ofxOceanodeNodeModel
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
    ofParameter<int>                    paramOffsetBeatDiv;
    ofParameter<int>                    paramOffsetBeatMult;
    ofParameter<float>                  paramGatePct;
    ofParameter<void>                   paramRestart;
    ofParameter<ofxPm::VideoFrame>      paramAuxFrame;
    

    ofxPm::VideoBufferNodeBased                 buffer;
    ofxPm::VideoHeaderNodeBased                 videoHeader;
    bool                                        restart;

    void                                loopTimeChanged(int& _i);
    void                                doLoopChanged(bool& _b);
    void                                doRestart();
    ofxOceanodeAbstractConnection* createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink) override;

private:

    // rendering
    VideoFrame                          myFrame;
    double                              loopDurationMs;
    double                              loopStartedAtMs;
    double                              loopDurationMsWhenTriggered;
    double                              BPMfactor;
    
    phasor                              phasor;
    
    
};
}

#endif // LOOPERFILTER_H
