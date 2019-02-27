
#ifndef VIDEOHEADERNODEBASED_H_INCLUDED
#define VIDEOHEADERNODEBASED_H_INCLUDED

#include "VideoSink.h"
#include "VideoSource.h"
#include "VideoBufferNodeBased.h"

#include "ofxOceanodeNodeModel.h"

// acts as a video source and sink
// controls the position in a buffer
// based on speed, delay, fps and loop in/out

namespace ofxPm{
    
class VideoHeaderNodeBased:public VideoSink, public VideoSource, public ofxOceanodeNodeModel{
public:
    VideoHeaderNodeBased();
    ~VideoHeaderNodeBased();

    void setup(ofxPm::VideoBufferNodeBased* _buff);
    void setupNodeBased();
    
    VideoBufferNodeBased *getBuffer();
    void        changedVideoBuffer(VideoBufferNodeBased* &videoBuffer);

    VideoFrame  getNextVideoFrame();
    Timestamp   getNextFrameTimestamp();
    //VideoFrame  getVideoFrame(int index);
	
    void setInMs(double _in);
    void setOutMs(double _out);
    
    double  getDelayMs(){return delayInMs;};
    void	setDelayMs(double _delayMs); // !! This is the main function to manipulate the video header !

    float	getFps();
    void	setFps(float fps);

    string	getInfo();
    void	setOffsetInFrames(int _o);
	
    double  getBufferSizeInMs(){return totalBufferSizeInMs;};
    
    // when global BPM changes, it will call this function so I can know the global bpm
    void    setBpm(float _bpm) override{myBPM=_bpm;};


    
protected:
	double			totalBufferSizeInMs;
    double			oneFrameMs;
    double			fps;				// framerate
    double 			inMs, outMs, lengthMs;	// expressed in ms from realtime ... in is bigger then out
	int				offsetFrames;				// to adjust av sync in frames
    float           myBPM;

    //bool            playing;
    
    
    //---------- TS HEADER CONTROL VARS

    Timestamp		nowTS;
    double          delayInMs;
    Timestamp       inTS, outTS;
    Timestamp       currentFrameTs;
    int             currentFrameIndex;
    
    // NODE BASED STUFF
    ofParameter<ofxPm::VideoBufferNodeBased*>    paramVideoBufferInput;
    ofParameter<float>                  paramDelayMs;
    ofParameter<bool>                   paramManualOrBPM;
    ofParameter<int>                    paramBeatDiv;
    ofParameter<int>                    paramBeatMult;

    int oldBufferSize;

    
    ofEventListener listener;
};
}
#endif // VIDEOHEADERNODEBASED_H_INCLUDED
