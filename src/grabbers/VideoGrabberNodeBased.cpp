

#include "VideoGrabberNodeBased.h"

namespace ofxPm{
    
    //-----------------------------------------------------------------------------------
    VideoGrabberNodeBased::VideoGrabberNodeBased(): ofxOceanodeNodeModel("Video Grabber")
    {
        //numGrabberDevices = VideoGrabberNodeBased::listDevices().size();
        // TODO : this is provisional !!
        numGrabberDevices = 2;
        
        parameters->add(paramDeviceId.set("DeviceId", 0, 0, numGrabberDevices-1));
        parameters->add(paramGrab.set("Grab",true));
        parameters->add(paramConnect.set("Connect"));
        parameters->add(paramResolutionX.set("Resolution X",640,0,1920));
        parameters->add(paramResolutionY.set("Resolution Y",480,0,1080));
        parameters->add(paramFps.set("FPS",60,0,60));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramConnect.addListener(this, &VideoGrabberNodeBased::connectToDevice);
        
        color = ofColor::darkGreen;

    }


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
                    //newFrame(getTexture());
                }
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
    void VideoGrabberNodeBased::connectToDevice()
    {
        cout << "VideoGrabberNodeBased::set Reconnect .... " << endl;
        
        VideoGrabberNodeBased::close();
        VideoGrabberNodeBased::listDevices();
        VideoGrabberNodeBased::setDeviceID(paramDeviceId);
        ofVideoGrabber::setDesiredFrameRate(paramFps);
        
        ofVideoGrabber::setup(paramResolutionX,paramResolutionY);

        VideoSource::setWidth(ofVideoGrabber::getWidth());
        VideoSource::setHeight(ofVideoGrabber::getHeight());
        cout << "VideoGrabberNodeBased::reconnecting to grabber device " << paramDeviceId << " at : " << paramResolutionX << " , " << paramResolutionY << " || FPS : " << paramFps << endl;
    }
    
}
