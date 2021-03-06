

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
        float                           getFps(){return getFps();};             // ugly! need to be able to override the fps of the grabber. ofVideoGrabber doesn't have a getFps !!
        void                            setFps(float fps);
        
        void presetSave(ofJson &json) override{
            json["FileString"] = files[paramFile];
        }
        
        void presetRecallAfterSettingParameters(ofJson &json) override{
            if(json.count("FileString") == 1){
                string fileString = json["FileString"];
                ptrdiff_t pos = find(files.begin(), files.end(), fileString) - files.begin();
                if(pos < files.size()) {
                    paramFile = pos;
                }
            }
        }

    private:
        float                               fps;
        void                                newFrame(ofPixels & frame);
        void                                newFrame(ofTexture & tex);
        VideoFrame                          frame;
        
        ofParameter<int>                    paramFile;
        ofParameter<bool>                   paramPlay;
        ofParameter<bool>                   paramScratch;
        ofParameter<float>                  paramHeader;
        ofParameter<int>                    paramRotation;
        ofParameter<bool>                   paramHFlip,paramVFlip;
        ofParameter<float>                  paramOpacity;
        
        void                                playChanged(bool & _b);
        void                                scratchChanged(bool & _b);
        void                                headerChanged(float & _f);
        
        ofFbo   fbo;
        
        int oldParamFile;
        
        vector<string> files;

        
        ofEventListeners listeners;
    };
}

#endif // VIDEOFILEGRABBER_H_INCLUDED
