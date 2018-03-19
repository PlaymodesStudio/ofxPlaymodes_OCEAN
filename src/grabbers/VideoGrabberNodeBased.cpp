

#include "VideoGrabberNodeBased.h"
#include "ofxOceanodeContainer.h"

namespace ofxPm{
    
    //-----------------------------------------------------------------------------------
    VideoGrabberNodeBased::VideoGrabberNodeBased(): ofxOceanodeNodeModel("Video Grabber")
    {
        numGrabberDevices = VideoGrabberNodeBased::listDevices().size();;
        
        parameters->add(paramDeviceId.set("DeviceId", 0, 0, numGrabberDevices-1));
        parameters->add(paramGrab.set("Grab",true));
        parameters->add(paramConnect.set("Connect"));
        parameters->add(paramResolutionX.set("Resolution X",640,0,1920));
        parameters->add(paramResolutionY.set("Resolution Y",480,0,1080));
        parameters->add(paramFps.set("FPS",60,0,60));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramConnect.addListener(this, &VideoGrabberNodeBased::connectToDevice);
        
    }
    
//    //-----------------------------------------------------------------------------------
//    ofxOceanodeAbstractConnection* VideoGrabberNodeBased::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink){
//        if(source.type() == typeid(ofParameter<ofxPm::VideoFrame*>).name()){
//            return c.connectConnection(source.cast<ofxPm::VideoFrame*>(), sink.cast<ofxPm::VideoFrame*>());
//        }
//    }


    //-----------------------------------------------------------------------------------
    VideoGrabberNodeBased::~VideoGrabberNodeBased()
    {
        cout << "VideoGrabberNodeBased::Destroying Grabber..." << endl;
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
            if(paramGrab)
            {
                ofVideoGrabber::update();
                if(ofVideoGrabber::isFrameNew())
                {
                    newFrame(getPixelsRef());
                }
            }            
        }
    }

    //-----------------------------------------------------------------------------------
    void VideoGrabberNodeBased::newFrame(ofPixels & pixels)
    {
        frame = VideoFrame::newVideoFrame(pixels);
//        parameters->get("Frame Output").cast<ofxPm::VideoFrame>() = frame;
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
    void VideoGrabberNodeBased::connectToDevice()
    {
        cout << "VideoGrabberNodeBased::set Reconnect .... " << endl;
        
        ofVideoGrabber::close();
        ofVideoGrabber::listDevices();
        ofVideoGrabber::setDeviceID(paramDeviceId);
        ofVideoGrabber::setDesiredFrameRate(paramFps);
        
        ofVideoGrabber::setup(paramResolutionX,paramResolutionY);
//        if(ofVideoGrabber::isInitialized())
//        {
//            ofAddListener(ofEvents().update, this, &VideoGrabberNodeBased::update);
//        }

        VideoSource::setWidth(ofVideoGrabber::getWidth());
        VideoSource::setHeight(ofVideoGrabber::getHeight());
        cout << "VideoGrabberNodeBased::reconnecting to grabber device " << paramDeviceId << " at : " << paramResolutionX << " , " << paramResolutionY << " || FPS : " << paramFps << endl;
    }
    
}