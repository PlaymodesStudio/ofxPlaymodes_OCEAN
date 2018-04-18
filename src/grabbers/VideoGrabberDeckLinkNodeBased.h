
#ifndef VIDEOGRABBERDECKLINKNODEBASED_H_INCLUDED
#define VIDEOGRABBERDECKLINKNODEBASED_H_INCLUDED

#include "VideoSource.h"
#include "ofMain.h"
#include "ofxDeckLink.h"
#include "ofxOceanodeNodeModel.h"



namespace ofxPm{
class VideoGrabberDeckLinkNodeBased: public VideoSource, public ofVideoGrabber,public ofxOceanodeNodeModel{
public:
    
    VideoGrabberDeckLinkNodeBased();
    
    virtual ~VideoGrabberDeckLinkNodeBased();

    bool initGrabber(int w, int h);

	VideoFrame getNextVideoFrame();
    
    float getFps();
    
	// ugly! need to be able to override the fps of the grabber. ofVideoGrabber doesn't have a getFps !!
	void setFps(float fps);
	
    void update(ofEventArgs &e);




private:
    void                    newFrame(ofPixels & frame);
	float                   fps;
	VideoFrame              frame;
    
    ofxDeckLinkAPI::Input   input;
    ofFbo                   fbo;
    ofPixels                pixels;
};
}

#endif // VIDEOGRABBERDECKLINK_H_INCLUDED
