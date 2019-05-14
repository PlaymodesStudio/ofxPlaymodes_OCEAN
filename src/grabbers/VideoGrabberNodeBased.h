

#ifndef VIDEOGRABBERNODEBASED_H_INCLUDED
#define VIDEOGRABBERNODEBASED_H_INCLUDED

#include "VideoSource.h"
#include "ofxOceanodeNodeModel.h"


namespace ofxPm{
    class VideoGrabberNodeBased: public VideoSource, public ofVideoGrabber, public ofxOceanodeNodeModel
    {
    public:
        VideoGrabberNodeBased();
        virtual ~VideoGrabberNodeBased();
        
        void                            update(ofEventArgs &e);
        VideoFrame                      getNextVideoFrame();
        float                           getFps();             // ugly! need to be able to override the fps of the grabber. ofVideoGrabber doesn't have a getFps !!
        void                            setFps(float fps);

    private:
        void                                newFrame(ofPixels & frame);
        void                                newFrame(ofTexture & tex);
        VideoFrame                          frame;
        
        ofParameter<int>                    paramDeviceId;
        ofParameter<int>                    paramResolutionX;
        ofParameter<int>                    paramResolutionY;
        ofParameter<int>                    rotation;
        ofParameter<bool>                   vFlip;
        ofParameter<bool>                   hFlip;
        ofParameter<int>                    paramFps;
        ofParameter<bool> showGuides;
        ofParameter<bool> waitForFrame;
        

        void                                setGrabberSettings(bool & _b);
        void                                selectedDevice(int &identifier);
        ofFbo                               fbo;

        ofEventListeners listeners;
    };
}

#endif // VIDEOGRABBERNODEBASED_H_INCLUDED
