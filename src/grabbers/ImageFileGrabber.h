

#ifndef IMAGEFILEGRABBER_H_INCLUDED
#define IMAGEFILEGRABBER_H_INCLUDED

#include "VideoSource.h"
#include "ofMain.h"
#include "ofxOceanodeNodeModel.h"


namespace ofxPm{
    class ImageFileGrabber: public VideoSource, public ofImage, public ofxOceanodeNodeModel
    {
    public:
        ImageFileGrabber();
        virtual ~ImageFileGrabber();
        
        void                            update(ofEventArgs &e);
        VideoFrame                      getNextVideoFrame();
        float                           getFps(){return getFps();};             // ugly! need to be able to override the fps of the grabber. ofVideoGrabber doesn't have a getFps !!
        void                            setFps(float fps);

    private:
        float                               fps;
        void                                newFrame(ofPixels & frame);
        void                                newFrame(ofTexture & tex);
        VideoFrame                          frame;
        
        ofParameter<int>                    paramFile;
        ofParameter<float>                  paramOpacity;
        ofParameter<int>                    paramRotation;
        ofParameter<bool>                   paramHFlip, paramVFlip;

        
//        void                                playChanged(bool & _b);
//        void                                scratchChanged(bool & _b);
//        void                                headerChanged(float & _f);
        
        ofFbo   fbo;

        
        ofEventListeners listeners;
    };
}

#endif // IMAGEFILEGRABBER_H_INCLUDED
