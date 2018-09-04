/*
 * VideoFilter.h
 *
 *  Created on: 19-des-2008
 *      Author: eloi
 */

#ifndef VIDEOFILTER_H_
#define VIDEOFILTER_H_

#include "VideoSource.h"
#include "VideoSink.h"

namespace ofxPm{
class VideoFilter : public VideoSink, public VideoSource {
public:

    VideoFilter();
	virtual ~VideoFilter();
    virtual void newVideoFrame(VideoFrame & frame){};
    virtual VideoFrame getNextVideoFrame()=0;
    virtual float getFps()=0;

};
}
#endif /* VIDEOFILTER_H_ */
