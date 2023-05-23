///*
// * AudioFrame.h
// *
// *  Created on: 09-oct-2008
// *      Author: arturo castro
// */
//
//
//#ifndef VIDEOGRABBERPS3EYENODEBASED_H_INCLUDED
//#define VIDEOGRABBERPS3EYENODEBASED_H_INCLUDED
//
//#include "VideoSource.h"
//#include "ofMain.h"
//#include "ps3eye.h"
//#include "ofxOceanodeNodeModel.h"
//        
//        
//        namespace ofxPm{
//class VideoGrabberPS3EyeNodeBased: public VideoSource, public ofVideoGrabber, public ofxOceanodeNodeModel{
//public:
//    VideoGrabberPS3EyeNodeBased();
//    virtual ~VideoGrabberPS3EyeNodeBased();
//
//    bool initGrabber(int w, int h);
//
//	VideoFrame getNextVideoFrame();
//    float getFps();
//	// ugly! need to be able to override the fps of the grabber. ofVideoGrabber doesn't have a getFps !!
//	void setFps(float fps);
//	
//    void update(ofEventArgs &e);
//
//    void setAutoWhiteBalance(bool &_b){if(eye!=nullptr) eye->setAutoWhiteBalance(_b);};
//    void setAutoGain(bool &_b){if(eye!=nullptr) eye->setAutogain(_b);};
//    void setExposure(int &_i){if(eye!=nullptr) eye->setExposure(_i);};
//    void setHue(int &_i){if(eye!=nullptr) eye->setHue((uint8_t)_i);};
//
//private:
//    void newFrame(ofPixels & frame);
//	float fps;
//	VideoFrame frame;
//    
//    ps3eye::PS3EYECam::PS3EYERef eye;
//    unsigned char * videoFrame;
//    ofTexture videoTexture;
//
//    // FEATURE NODES
//    ofParameter<int>                    device;
//    ofParameter<int>                    paramExposure;
//    ofParameter<int>                    paramHue;
//    ofParameter<bool>                   paramAutoGain;
//    ofParameter<bool>                   paramAutoWB;
//    ofParameter<void>                   paramConnect;
//    
//    void                                connectToDevice();
//    
//    ofEventListeners listeners;
//    
//};
//}
//
//#endif // VIDEOGRABBERPS3EYENODEBASED_H_INCLUDED
