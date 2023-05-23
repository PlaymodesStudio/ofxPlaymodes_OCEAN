

#include "VideoFileGrabber.h"
//#include "ofGstVideoPlayer.h"


namespace ofxPm{
    
    //-----------------------------------------------------------------------------------
    VideoFileGrabber::VideoFileGrabber(): ofxOceanodeNodeModel("Video File")
    {
        //ofVideoPlayer::setPlayer(ofPtr<ofGstVideoPlayer>(new ofGstVideoPlayer));

        color = ofColor::darkGreen;
        
        ofDirectory dir;
        dir.open("movies");
        if(!dir.exists()){
            dir.createDirectory("movies");
        }
        int size = dir.listDir();
        dir.sort();
        files = {"None"};
        for(int i = 0; i < size ; i++){
          files.push_back(dir.getName(i));
        }
        addParameterDropdown(paramFile, "File", 0, files);
        addParameter(paramPlay.set("Play",true));
        addParameter(paramScratch.set("Scratch",false));
        addParameter(paramHeader.set("Header",0.0,0.0,1.0));
        addParameter(paramRotation.set("Rotation 90x",0,0,3));
        addParameter(paramVFlip.set("Vertical Flip", false));
        addParameter(paramHFlip.set("Horizontal Flip", false));
        addParameter(paramOpacity.set("Opacity",1.0));
        addParameter(paramFrameOut.set("Frame Output", frame));
        
        listeners.push(paramScratch.newListener(this, &VideoFileGrabber::scratchChanged));
        listeners.push(paramHeader.newListener(this, &VideoFileGrabber::headerChanged));
        listeners.push(paramPlay.newListener(this, &VideoFileGrabber::playChanged));
        listeners.push(paramRotation.newListener([this](int &i){
            fbo.clear();
        }));
        listeners.push(paramFile.newListener([this, dir](int &index){
            if(index == 0){
                ofVideoPlayer::closeMovie();
            }else if(oldParamFile != index){
                ofVideoPlayer::load(dir.getPath(index-1));
                ofVideoPlayer::play();
                ofVideoPlayer::setLoopState(OF_LOOP_NORMAL);
                ofVideoPlayer::setVolume(1.0);
            }
            oldParamFile = index;
        }));
        oldParamFile = 0;
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
        if(ofVideoPlayer::isPlaying() || ofVideoPlayer::isPaused())
        {
            ofVideoPlayer::update();
            if(true) //ofVideoPlayer::isFrameNew())
            {
                if(!fbo.isAllocated()){
                    if(paramRotation % 2 == 0){
                        fbo.allocate(ofVideoPlayer::getWidth(), ofVideoPlayer::getHeight(), GL_RGB);
                        cout << " FBOKK" << endl;
                    }else{
                        fbo.allocate(ofVideoPlayer::getHeight(), ofVideoPlayer::getWidth(), GL_RGB);
                        cout << " FBOKK 2" << endl;
                    }
                }
                if(paramOpacity>0.0)
                {
                    fbo.begin();
                    {
                        ofClear(0, 0, 0);
//                        ofTexture tex;
//                        tex.loadData(getPixels());
                        ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
                        ofScale(paramHFlip ? -1 : 1, paramVFlip ? -1 : 1);
                        ofRotateDeg(90*paramRotation);
                        ofTranslate(-ofVideoPlayer::getWidth()/2, -ofVideoPlayer::getHeight()/2);
                        ofSetColor(255.0 * paramOpacity);
//                        tex.draw(0, 0);
                        ofVideoPlayer::draw(0,0);
                    }
                    fbo.end();
                    newFrame(fbo.getTexture());
                }
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
