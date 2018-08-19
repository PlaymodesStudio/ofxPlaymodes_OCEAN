

#include "VideoFileGrabber.h"

namespace ofxPm{
    
    //-----------------------------------------------------------------------------------
    VideoFileGrabber::VideoFileGrabber(): ofxOceanodeNodeModel("Video File")
    {
        color = ofColor::darkGreen;

        ofVideoPlayer::load("test2.mov");
        ofVideoPlayer::play();
        ofVideoPlayer::setVolume(0.0);
        
        parameters->add(paramPlay.set("Play",true));
        parameters->add(paramScratch.set("Scratch",false));
        parameters->add(paramHeader.set("Header",0.0,0.0,1.0));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        paramScratch.addListener(this, &VideoFileGrabber::scratchChanged);
        paramHeader.addListener(this, &VideoFileGrabber::headerChanged);
        paramPlay.addListener(this, &VideoFileGrabber::playChanged);

        
    }


    //-----------------------------------------------------------------------------------
    VideoFileGrabber::~VideoFileGrabber()
    {
        cout << "VideoFileGrabber::Destroying Grabber..." << endl;
        //ofVideoPlayer::close();
    }

    //-----------------------------------------------------------------------------------
    VideoFrame VideoFileGrabber::getNextVideoFrame()
    {
        return frame;
    }

    //-----------------------------------------------------------------------------------
    void VideoFileGrabber::update(ofEventArgs &e)
    {
        
        //if(ofVideoPlayer::isInitialized())
        if(ofVideoPlayer::isPlaying() || ofVideoPlayer::isPaused())
        {
            ofVideoPlayer::update();
            if(ofVideoPlayer::isFrameNew())
            {
                newFrame(ofVideoPlayer::getPixels());
                //newFrame(getTexture());
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void VideoFileGrabber::newFrame(ofPixels & pixels)
    {
        frame = VideoFrame::newVideoFrame(pixels);
        paramFrameOut = frame;
    }
    //-----------------------------------------------------------------------------------
    void VideoFileGrabber::newFrame(ofTexture & tex)
    {
        frame = VideoFrame::newVideoFrame(tex);
        paramFrameOut = frame;
    }

    //-----------------------------------------------------------------------------------
    void VideoFileGrabber::playChanged(bool & _b)
    {
        if(_b)
        {
            cout << "PLAY !!"  << endl;
            ofVideoPlayer::play();
        }
        else
        {
            cout << "STOP !!"  << endl;
            ofVideoPlayer::stop();
        }
    }
    
    //-----------------------------------------------------------------------------------
    void VideoFileGrabber::scratchChanged(bool & _b)
    {
        if(_b)
        {
            ofVideoPlayer::setPaused(_b);
        }
        else
        {
            ofVideoPlayer::setPaused(_b);
        }
    }
    
    //-----------------------------------------------------------------------------------
    void VideoFileGrabber::headerChanged(float & _f)
    {
        if(paramScratch) ofVideoPlayer::setPosition(_f);
    }


}
