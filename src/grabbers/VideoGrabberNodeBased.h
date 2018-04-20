

#ifndef VIDEOGRABBERNODEBASED_H_INCLUDED
#define VIDEOGRABBERNODEBASED_H_INCLUDED

#include "VideoSource.h"
#include "ofMain.h"
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
        int                                 numGrabberDevices;
        
        ofParameter<bool>                   paramSettings;
        ofParameter<void>                   paramConnect;
        ofParameter<bool>                   paramGrab;
        ofParameter<int>                    paramDeviceId;
        ofParameter<int>                    paramResolutionX;
        ofParameter<int>                    paramResolutionY;
        ofParameter<int>                    paramFps;

        void                                setGrabberSettings(bool & _b);
        void                                connectToDevice();

    };
}

#endif // VIDEOGRABBERNODEBASED_H_INCLUDED
