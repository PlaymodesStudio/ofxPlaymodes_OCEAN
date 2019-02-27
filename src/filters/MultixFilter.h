/*
 * AudioFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef MULTIXFILTER_H
#define MULTIXFILTER_H

#include "VideoBufferNodeBased.h"
#include "VideoHeaderNodeBased.h"
#include "ofxOceanodeNodeModel.h"

#include "ofMain.h"

namespace ofxPm{
        
class MultixFilter: public VideoSource,public ofxOceanodeNodeModel
{
public:
    MultixFilter();
    virtual ~MultixFilter();

    void    setupNodeBased();
    void    update(ofEventArgs &e) override;

    void                            updateValuesMs(vector<float> _vf);
    void                            drawIntoFbo(int x, int y,int w, int h);

    ofxPm::VideoBufferNodeBased*    getVideoBuffer() const;
    void                            changedVideoBuffer(ofxPm::VideoBufferNodeBased* &videoBuffer);
    void                            setVideoHeader(vector<ofxPm::VideoHeaderNodeBased*> videoHeader);
    
    ofMutex                         mutex;

    // +
    void                            newVideoFrame(VideoFrame & _frame);
    VideoFrame                      getNextVideoFrame();
    float                           getFps(){return paramVideoBufferInput.get()->getFps();};
    void                            setMinmaxBlend(bool &_b){paramMinMaxBlend = _b;}
    void                            setOffsetBeatDiv(int _n){paramOffsetBeatDiv = _n;};
    void                            setOffsetBeatMult(int _n){paramOffsetBeatMult = _n;};

    void                            recalculate(int &_i);
    void                            changedManualOffsetMs(float &f);
    //    void                                    changedDistributionVector(vector<float> &_b);

    // when global BPM changes, it will call this function so I can know the global bpm
    void    setBpm(float _bpm) override{myBPM=_bpm;int i=0;recalculate(i);};

protected:
    
    ofxPm::VideoHeaderNodeBased         videoHeader;
    vector<float>                       multixDelaysInMs;
    float                               myBPM;

    ofParameter<int>                    paramNumHeaders;
    ofParameter<bool>                   paramMinMaxBlend;
    //ofParameter<int>                    paramOpacityMode;
    //ofParameter<bool>                   paramLinearDistribution;
    ofParameter<bool>                   paramUseBPM;
    ofParameter<float>                  paramManualOffsetMs;
    ofParameter<int>                    paramOffsetBeatDiv;
    ofParameter<int>                    paramOffsetBeatMult;
    ofParameter<vector<float>>          paramOpacityVector;
    ofParameter<int>                    paramOversize;
    ofParameter<ofxPm::VideoBufferNodeBased*>    paramVideoBufferInput;
    //ofParameter<bool>                   paramLinearDistribution;
    //ofParameter<vector<float>>          paramDistributionVector;
    
private:

    // rendering
    VideoFrame      frame;
    ofFbo           fbo;
    int             oldBufferSize;
    int             desiredBufferSize;
    
    ofEventListeners listeners;

};
}

#endif // MULTIXFILTER_H
