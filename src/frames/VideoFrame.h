/*
 * VideoFrame.h
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#ifndef VIDEOFRAME_H_
#define VIDEOFRAME_H_

#include "Frame.h"
#include "ofPixels.h"
#include "ofTexture.h"
#include "ofEvents.h"
#include "VideoFormat.h"
#include <map>

namespace ofxPm{
class VideoFrame: public ofxPm::Frame, public ofEventArgs {
public:
	VideoFrame();
    
    // create a video frame from an ofPixels
    VideoFrame(const ofPixels & videoFrame);
    VideoFrame(ofTexture & videoFrame);
    VideoFrame(ofFbo & videoFrame);

    static VideoFrame newVideoFrame(const ofPixels & videoFrame);
	static VideoFrame newVideoFrame(ofTexture & videoFrame);
	static VideoFrame newVideoFrame(ofFbo & videoFrame);
	static VideoFrame newVideoFrame(VideoFrame videoFrame);

	virtual ~VideoFrame();

    // returns pixels array
	ofPixels & getPixelsRef();
	ofTexture & getTextureRef();
	ofFbo & getFboRef();

	int getWidth();
	int getHeight();

//	operator void*();

	void setTextureOnly(bool texOnly);

	static int getPoolSize(const VideoFormat & format);
	static int getTotalNumFrames();


    bool isNull()
    {
        return (data.get() == NULL);
    }
    
    bool isNullPtr()
    {
        return (data == nullptr);
    }
    
private:
    
    
	class Obj;
	ofPtr<Obj> data;
    static int total_num_frames;
    static map<VideoFormat,vector< ofPtr<Obj> >  > pool;
    static ofMutex poolMutex;
    static void poolDeleter(Obj * obj);

};
}
#endif /* VIDEOFRAME_H_ */
