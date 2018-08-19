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
//    MultixFilter(ofxPm::VideoBuffer & buffer, int numHeaders=2);

    virtual ~MultixFilter();

//    void    setup(ofxPm::VideoBuffer & buffer, int numHeaders=2);
    void    setupNodeBased();
    void update(ofEventArgs &e) override;


    void    updateValuesPct(vector<float> _vf);
    void    updateValuesMs(vector<float> _vf);
    void    drawIntoFbo(int x, int y,int w, int h);

    void    setNumHeaders(int numHeaders);
    int     getNumHeaders();

    
    ofxPm::VideoBufferNodeBased *getVideoBuffer() const;
    void    changedVideoBuffer(ofxPm::VideoBufferNodeBased* &videoBuffer);
    void    setVideoHeader(vector<ofxPm::VideoHeaderNodeBased*> videoHeader);
    
    ofMutex     mutex;
    bool        isMinmaxBlend() const;
    
    // + 
    void                                    newVideoFrame(VideoFrame & _frame);
//    ofEvent<VideoFrame>                     newFrameEvent;
    VideoFrame                              getNextVideoFrame();
    float                                   getFps(){return paramVideoBufferInput.get()->getFps();};

    void                                    setMinmaxBlend(bool &_b){paramMinMaxBlend = _b;}
    void                                    setOpacityMode(int &_m){paramOpacityMode=_m;};
    void                                    setOffsetBeatDiv(int _n){paramOffsetBeatDiv = _n;};
    void                                    setOffsetBeatMult(int _n){paramOffsetBeatMult = _n;};

    void                                    recalculate(int &_i);
    void                                    changedDistributionVector(vector<float> &_b);
    
    // when global BPM changes, it will call this function so I can know the global bpm
    void    setBpm(float _bpm) override{myBPM=_bpm;cout << "MULTIX BPM" << endl;};

protected:
    
    ofxPm::VideoHeaderNodeBased         videoHeader;
    vector<float>                       multixDelaysInMs;
    float                               myBPM;
    //NodeBased

    ofParameter<int>                    paramNumHeaders;
    ofParameter<int>                    paramOpacityMode;
    ofParameter<bool>                   paramMinMaxBlend;
    ofParameter<bool>                   paramLinearDistribution;
    ofParameter<bool>                   paramUseBPM;
    ofParameter<float>                  paramManualOffsetMs;
    ofParameter<int>                    paramOffsetBeatDiv;
    ofParameter<int>                    paramOffsetBeatMult;
    ofParameter<ofxPm::VideoBufferNodeBased*>    paramVideoBufferInput;
    ofParameter<vector<float>>          paramDistributionVector;

    
private:

    int             numHeaders;
    glm::vec2       fboHasToBeAllocated;

    // rendering
    VideoFrame      frame;
    ofFbo           fbo;
    bool            minmaxBlend;
    int             opacityMode;

};
}

#endif // MULTIXFILTER_H
