

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
        addParameterToGroupAndInfo(topMask.set("Top Mask", 0, 0, 1280)).isSavePreset = false;
        addParameterToGroupAndInfo(showGuides.set("Show Guides", false)).isSavePreset = false;
        addParameterToGroupAndInfo(waitForFrame.set("Wait for Frame", true)).isSavePreset = false;
        //addParameterToGroupAndInfo(paramFps.set("FPS",60,0,60));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        listeners.push(paramDeviceId.newListener(this, &VideoGrabberNodeBased::selectedDevice));
        listeners.push(rotation.newListener([this](int &r){
            if(r % 2 == 1){
                if(fbo.getWidth() != paramResolutionY || fbo.getHeight() != paramResolutionX){
                    fbo.allocate(paramResolutionY, paramResolutionX, GL_RGB);
                }
            }else{
                if(fbo.getWidth() != paramResolutionX || fbo.getHeight() != paramResolutionY){
                    fbo.allocate(paramResolutionX, paramResolutionY, GL_RGB);
                }
            }
        }));
        
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
            if(waitForFrame){
                while(!ofVideoGrabber::isFrameNew()){
                    ofVideoGrabber::update();
                }
            }
            else{
                if(!ofVideoGrabber::isFrameNew()) return;
            }
            {
                fbo.begin();
                ofClear(0, 0, 0);
//                ofTexture tex;
//                tex.loadData(getPixels());
                ofPushMatrix();
                ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
                ofScale(hFlip ? -1 : 1, vFlip ? -1 : 1);
                ofRotateDeg(90*rotation);
                ofTranslate(-paramResolutionX/2, -paramResolutionY/2);
                //tex.draw(0, 0);
                ofVideoGrabber::draw(0 ,0);
                ofPopMatrix();
                ofPushStyle();
                ofSetColor(0);
                ofDrawRectangle(0, 0, 720, topMask);
                ofPopStyle();
                if(showGuides){
                    ofDrawLine(fbo.getWidth()/2, 0, fbo.getWidth()/2, fbo.getHeight());
                    ofDrawLine(0, fbo.getHeight()/2, fbo.getWidth(), fbo.getHeight()/2);
                    ofDrawLine(0, 0, fbo.getWidth(), fbo.getHeight());
                    ofDrawLine(fbo.getWidth(), 0, 0, fbo.getHeight());
                }
                fbo.end();
                newFrame(fbo.getTexture());
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
            ofVideoGrabber::setup(paramResolutionX,paramResolutionY, true);
            ofVideoGrabber::setDesiredFrameRate(ofGetTargetFrameRate());
            if(rotation % 2 == 0){
                fbo.allocate(paramResolutionX, paramResolutionY, GL_RGB);
            }else{
                fbo.allocate(paramResolutionY, paramResolutionX, GL_RGB);
            }
            VideoSource::setWidth(ofVideoGrabber::getWidth());
            VideoSource::setHeight(ofVideoGrabber::getHeight());
        }
    }
    
}
