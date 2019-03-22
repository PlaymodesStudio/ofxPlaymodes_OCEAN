

#include "VideoGrabberNodeBased.h"

namespace ofxPm{
    
    //-----------------------------------------------------------------------------------
    VideoGrabberNodeBased::VideoGrabberNodeBased(): ofxOceanodeNodeModel("Video Grabber")
    {
        vector<string> videoDevices = {"None"};
        for(auto &d : ofVideoGrabber::listDevices()){
            videoDevices.push_back(d.deviceName);
        }
        
        addParameterToGroupAndInfo(createDropdownAbstractParameter("Device", videoDevices, paramDeviceId)).isSavePreset = false;
        
        addParameterToGroupAndInfo(paramResolutionX.set("Resolution X",1280,0,1920)).isSavePreset = false;
        addParameterToGroupAndInfo(paramResolutionY.set("Resolution Y",720,0,1080)).isSavePreset = false;
        addParameterToGroupAndInfo(rotation.set("Rotation 90x",0,0,3)).isSavePreset = false;
        addParameterToGroupAndInfo(vFlip.set("Vertical Flip", false)).isSavePreset = false;
        addParameterToGroupAndInfo(hFlip.set("Horizontal Flip", false)).isSavePreset = false;
        addParameterToGroupAndInfo(paramFps.set("FPS",60,0,60));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        listener = paramDeviceId.newListener(this, &VideoGrabberNodeBased::selectedDevice);
        
        color = ofColor::darkGreen;
    }


    //-----------------------------------------------------------------------------------
    VideoGrabberNodeBased::~VideoGrabberNodeBased()
    {
        ofVideoGrabber::close();
    }

    //-----------------------------------------------------------------------------------
    VideoFrame VideoGrabberNodeBased::getNextVideoFrame()
    {
        return frame;
    }

    //-----------------------------------------------------------------------------------
    void VideoGrabberNodeBased::update(ofEventArgs &e)
    {
        if(ofVideoGrabber::isInitialized())
        {
            ofVideoGrabber::update();
            if(ofVideoGrabber::isFrameNew())
            {
                ofPixels &p = getPixels();
                p.rotate90(rotation);
                p.mirror(vFlip, hFlip);
                newFrame(p);
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void VideoGrabberNodeBased::newFrame(ofPixels & pixels)
    {
        frame = VideoFrame::newVideoFrame(pixels);
        paramFrameOut = frame;
    }
    //-----------------------------------------------------------------------------------
    void VideoGrabberNodeBased::newFrame(ofTexture & tex)
    {
        frame = VideoFrame::newVideoFrame(tex);
        paramFrameOut = frame;
    }

    //------------------------------------------------------
    float VideoGrabberNodeBased::getFps()
    {
        return paramFps;
    }
	
    void VideoGrabberNodeBased::setFps(float fps)
    {
        this->paramFps = fps;
    }

    //------------------------------------------------------
    void VideoGrabberNodeBased::setGrabberSettings(bool & _b)
    {
        ofVideoGrabber::videoSettings();
    }
    //------------------------------------------------------
//    void VideoGrabberNodeBased::connectToDevice()
//    {
//        cout << "VideoGrabberNodeBased::set Reconnect .... " << endl;
//
//        VideoGrabberNodeBased::close();
//        VideoGrabberNodeBased::listDevices();
//        VideoGrabberNodeBased::setDeviceID(paramDeviceId);
//        ofVideoGrabber::setDesiredFrameRate(paramFps);
//
//        ofVideoGrabber::setup(paramResolutionX,paramResolutionY);
//
//        VideoSource::setWidth(ofVideoGrabber::getWidth());
//        VideoSource::setHeight(ofVideoGrabber::getHeight());
//        cout << "VideoGrabberNodeBased::reconnecting to grabber device " << paramDeviceId << " at : " << paramResolutionX << " , " << paramResolutionY << " || FPS : " << paramFps << endl;
//    }
    //---------------------------------------------------------
    void VideoGrabberNodeBased::selectedDevice(int &identifier){
        close();
        if(identifier > 0){
            setDeviceID(identifier-1);
            ofVideoGrabber::setup(paramResolutionX,paramResolutionY);
            VideoSource::setWidth(ofVideoGrabber::getWidth());
            VideoSource::setHeight(ofVideoGrabber::getHeight());
        }
    }
    
}
