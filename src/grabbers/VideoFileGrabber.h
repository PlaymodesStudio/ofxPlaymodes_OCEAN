

#ifndef VIDEOFILEGRABBER_H_INCLUDED
#define VIDEOFILEGRABBER_H_INCLUDED

#include "VideoSource.h"
#include "ofMain.h"
#include "ofxOceanodeNodeModel.h"


namespace ofxPm{
    class VideoFileGrabber: public VideoSource, public ofVideoPlayer, public ofxOceanodeNodeModel
    {
    public:
        VideoFileGrabber();
        virtual ~VideoFileGrabber();
        
        void                            update(ofEventArgs &e);
        VideoFrame                      getNextVideoFrame();
        float                           getFps(){getFps();};             // ugly! need to be able to override the fps of the grabber. ofVideoGrabber doesn't have a getFps !!
        void                            setFps(float fps);

    private:
        float                               fps;
        void                                newFrame(ofPixels & frame);
        void                                newFrame(ofTexture & tex);
        VideoFrame                          frame;
        
        ofParameter<bool>                   paramPlay;
        ofParameter<bool>                   paramScratch;
        ofParameter<float>                  paramHeader;
        
        void                                playChanged(bool & _b);
        void                                scratchChanged(bool & _b);
        void                                headerChanged(float & _f);

        

    };
}

#endif // VIDEOFILEGRABBER_H_INCLUDED