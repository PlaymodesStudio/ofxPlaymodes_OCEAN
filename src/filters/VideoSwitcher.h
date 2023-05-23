
/*
 * VideoMixerGPU.h
 *
 *  Created on: Jan 18, 2013
 *      Author: arturo
 */

#ifndef VIDEOSWITCHER_H_
#define VIDEOSWITCHER_H_

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    
    class VideoSwitcher: public VideoFilter,public ofxOceanodeNodeModel {
    public:
        VideoSwitcher();
        virtual ~VideoSwitcher();
        
        void setupNodeBased();
        void update(ofEventArgs &e) override;
        
        
        VideoFrame getNextVideoFrame(){return paramFrameIn2;};
        void newVideoFrame(VideoFrame & frame);
        void newVideoFrame2(VideoFrame & frame2);
        float getFps(){return fps;};
        
    private:
        
        float fps;
        ofParameter<ofxPm::VideoFrame>      paramFrameIn2;
        ofParameter<int>                    paramSwitchTo;
        
        ofEventListeners listeners;
    };
}

#endif /* VIDEOSWITCHER_H_ */
