

#include "VideoFileGrabber.h"

namespace ofxPm{
    
    //-----------------------------------------------------------------------------------
    VideoFileGrabber::VideoFileGrabber(): ofxOceanodeNodeModel("Video File")
    {
        color = ofColor::darkGreen;
        
        ofDirectory dir;
        dir.open("movies");
        if(!dir.exists()){
            dir.createDirectory("movies");
        }
        int size = dir.listDir();
        dir.sort();
        vector<string> files = {"None"};
        for(int i = 0; i < size ; i++){
          files.push_back(dir.getName(i));
        }
        addParameterToGroupAndInfo(createDropdownAbstractParameter("File", files, paramFile)).isSavePreset = false;
        addParameterToGroupAndInfo(paramPlay.set("Play",true)).isSavePreset = false;
        addParameterToGroupAndInfo(paramScratch.set("Scratch",false)).isSavePreset = false;
        addParameterToGroupAndInfo(paramHeader.set("Header",0.0,0.0,1.0)).isSavePreset = false;
        addParameterToGroupAndInfo(rotation.set("Rotation 90x",0,0,3)).isSavePreset = false;
        addParameterToGroupAndInfo(vFlip.set("Vertical Flip", false)).isSavePreset = false;
        addParameterToGroupAndInfo(hFlip.set("Horizontal Flip", false)).isSavePreset = false;
        addParameterToGroupAndInfo(sizeInFrames.set("Size In Frames", 0));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        listeners.push(paramScratch.newListener(this, &VideoFileGrabber::scratchChanged));
        listeners.push(paramHeader.newListener(this, &VideoFileGrabber::headerChanged));
        listeners.push(paramPlay.newListener(this, &VideoFileGrabber::playChanged));
        listeners.push(rotation.newListener([this](int &i){
            fbo.clear();
        }));
        listeners.push(paramFile.newListener([this, dir](int &index){
            ofVideoPlayer::closeMovie();
            if(index != 0){
                ofVideoPlayer::load(dir.getPath(index-1));
//                if(paramScratch) ofVideoPlayer::setPosition(paramHeader);
//                ofVideoPlayer::play();
                ofVideoPlayer::setLoopState(OF_LOOP_NONE);
                ofVideoPlayer::stop();
                ofVideoPlayer::setPaused(true);
                ofVideoPlayer::setSpeed(0);
//                ofVideoPlayer::setVolume(0.0);
//                sizeInFrames = ofVideoPlayer::getTotalNumFrames();
            }
        }));
        
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
        
        if(ofVideoPlayer::isPlaying() || ofVideoPlayer::isPaused())
        {
            ofVideoPlayer::update();
            if(ofVideoPlayer::isFrameNew())
            {
//                if(!fbo.isAllocated()){
//                    if(rotation % 2 == 0){
//                        fbo.allocate(ofVideoPlayer::getWidth(), ofVideoPlayer::getHeight(), GL_RGB);
//                    }else{
//                        fbo.allocate(ofVideoPlayer::getHeight(), ofVideoPlayer::getWidth(), GL_RGB);
//                    }
//                }
//                fbo.begin();
//                ofClear(0, 0, 0);
//                ofTexture tex;
//                tex.loadData(getPixels());
//                ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
//                ofScale(hFlip ? -1 : 1, vFlip ? -1 : 1);
//                ofRotateDeg(90*rotation);
//                ofTranslate(-ofVideoPlayer::getWidth()/2, -ofVideoPlayer::getHeight()/2);
//                tex.draw(0, 0);
//                fbo.end();
                newFrame(getPixels());
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
