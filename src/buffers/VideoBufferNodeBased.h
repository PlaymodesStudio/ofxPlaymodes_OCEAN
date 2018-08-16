
#ifndef VIDEOBUFFERNODEBASED_H_
#define VIDEOBUFFERNODEBASED_H_

#include "VideoFrame.h"
#include "pmUtils.h"
#include "Buffer.h"
#include "VideoSource.h"
#include "VideoSink.h"
#include "map"
#include "deque"

#include "ofxOceanodeNodeModel.h"


namespace ofxPm{
class VideoBufferNodeBased: public Buffer, public VideoSink, public VideoSource, public ofxOceanodeNodeModel{
public:
	VideoBufferNodeBased();
	virtual ~VideoBufferNodeBased();

    void setupNodeBased(int size, bool allocateOnSetup=false);

    // of working in threaded mode,
    // call buffer->lock() buffer->unlock()
    // to call this methods thread sage
	VideoFrame getVideoFrame(int position);     // frame number in the buffer
	VideoFrame getVideoFrame(TimeDiff time);    // frame at n microseconds from the end of the buffer
	VideoFrame getVideoFrame(float pct);        // % of the buffer

	VideoFrame getNextVideoFrame();               // the last video frame in the buffer


    // TS
    VideoFrame  getVideoFrame(Timestamp ts);     // get closer frame to given TimeStamp
    int         getLastGivenPosition(){return lastGivenPosition;};
    int         lastGivenPosition;
    Timestamp   getFirstFrameTimestamp(){return frames[frames.size()-1].getTimestamp();};
    Timestamp   getLastFrameTimestamp(){return frames[0].getTimestamp();};
    // TSx


	virtual void newVideoFrame(VideoFrame &frame);  // for notification of new frame event

    unsigned int getSizeInFrames();
	unsigned int getMaxSize();                         // max size of the buffer

	float getFps();                                   // fps of the video source

    Timestamp getLastTimestamp();                 // timestamp for the last frame
//    TimeDiff getTotalTime();                      // total time of the buffer in microseconds
//    Timestamp getInitTime();                      // time of the first frame in the recording

	//double getRealFPS();                             // aprox fps from source
	//int currentPos;                                 // currentPos in the buffer

    void stop();                                    // stop receiving new frames
    void resume();                                  // continue receiving new frames
    bool isStopped();

    void clear();
    
    void changedIsRecording(bool& _b);

protected:
    deque<VideoFrame> frames;


    Timestamp startRecordingTs;
    Timestamp stopRecordingTs;
    Timestamp::TimeVal lastFrameTimeValEpochMs;
    Timestamp::TimeVal lastFrameTimeValEpochMsAtStop;

    bool stopped;
    unsigned int maxSize;
    int framesOneSec;
    int64_t microsOneSec;
    //double realFps;
    
    
    ofParameter<ofxPm::VideoBufferNodeBased*>   paramVideoBufferOut;
    ofParameter<bool>                           paramIsRecording;
    ofParameter<float>                          paramFPS;
    
    
};
}
#endif /* VIDEOBUFFERNODEBASED_H_ */
