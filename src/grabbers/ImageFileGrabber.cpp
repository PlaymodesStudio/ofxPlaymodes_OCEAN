

#include "ImageFileGrabber.h"

namespace ofxPm{
    
    //-----------------------------------------------------------------------------------
    ImageFileGrabber::ImageFileGrabber(): ofxOceanodeNodeModel("Image File")
    {
        color = ofColor::darkGreen;
        
        ofDirectory dir;
        dir.open("images");
        if(!dir.exists()){
            dir.createDirectory("images");
        }
        int size = dir.listDir();
        dir.sort();
        vector<string> files = {"None"};
        for(int i = 0; i < size ; i++){
          files.push_back(dir.getName(i));
        }
        addParameterDropdown(paramFile, "File", 0, files);
        //addParameterToGroupAndInfo(opacity.set("Opacity", false)).isSavePreset = false;
        addParameter(paramRotation.set("Rotation 90x",0,0,3));
        addParameter(paramVFlip.set("Vertical Flip", false));
        addParameter(paramHFlip.set("Horizontal Flip", false));
        addParameter(paramOpacity.set("Opacity", 1.0));
        addParameter(paramFrameOut.set("Frame Output", frame));
        
//        listeners.push(paramScratch.newListener(this, &ImageFileGrabber::scratchChanged));
//        listeners.push(paramHeader.newListener(this, &ImageFileGrabber::headerChanged));
//        listeners.push(paramPlay.newListener(this, &ImageFileGrabber::playChanged));
//        listeners.push(rotation.newListener([this](int &i){
//            fbo.clear();
//        }));
        listeners.push(paramFile.newListener([this, dir](int &index){
            if(index == 0){
                //ofVideoPlayer::closeMovie();
            }else{
                ofImage::load(dir.getPath(index-1));
                //fbo.allocate(ofImage::getWidth(), ofImage::getHeight(), GL_RGB);
                //ofVideoPlayer::play();
                //ofVideoPlayer::setLoopState(OF_LOOP_NORMAL);
                //ofVideoPlayer::setVolume(0.0);
            }
        }));
        
    }


    //-----------------------------------------------------------------------------------
    ImageFileGrabber::~ImageFileGrabber()
    {
        cout << "ImageFileGrabber::Destroying Grabber..." << endl;
        //ofVideoPlayer::close();
    }

    //-----------------------------------------------------------------------------------
    VideoFrame ImageFileGrabber::getNextVideoFrame()
    {
        return frame;
    }

    //-----------------------------------------------------------------------------------
    void ImageFileGrabber::update(ofEventArgs &e)
    {

        if(!fbo.isAllocated() && ofImage::isAllocated() )
        {
            cout << "ImageFileGrabber : allocating FBO on update. From Image : " << ofImage::getWidth() << " , " << ofImage::getHeight() << endl;
            fbo.allocate(ofImage::getWidth(), ofImage::getHeight(), GL_RGB);
            cout << "ImageFileGrabber : allocated FBO : " << fbo.getWidth() << " , " << fbo.getHeight() << endl;
        }
        else
        {
            if(paramOpacity>0.0)
            {
                fbo.begin();
                {
                    ofClear(0, 0, 0);
                    ofTexture tex;
                    tex.loadData(getPixels());
                    ofTranslate(fbo.getWidth()/2, fbo.getHeight()/2);
                    ofScale(paramHFlip ? -1 : 1, paramVFlip ? -1 : 1);
                    ofRotateDeg(90*paramRotation);
                    ofTranslate(-ofImage::getWidth()/2, -ofImage::getHeight()/2);
                    ofSetColor(255.0 * paramOpacity);
                    tex.draw(0, 0);
                }
                fbo.end();
                newFrame(fbo.getTexture());
            }
        }
    }

    //-----------------------------------------------------------------------------------
    void ImageFileGrabber::newFrame(ofPixels & pixels)
    {
        frame = VideoFrame::newVideoFrame(pixels);
        paramFrameOut = frame;
    }
    //-----------------------------------------------------------------------------------
    void ImageFileGrabber::newFrame(ofTexture & tex)
    {
        frame = VideoFrame::newVideoFrame(tex);
        paramFrameOut = frame;
    }

//    //-----------------------------------------------------------------------------------
//    void ImageFileGrabber::playChanged(bool & _b)
//    {
//        if(_b)
//        {
//            cout << "PLAY !!"  << endl;
//            ofVideoPlayer::play();
//        }
//        else
//        {
//            cout << "STOP !!"  << endl;
//            ofVideoPlayer::stop();
//        }
//    }
//
//    //-----------------------------------------------------------------------------------
//    void ImageFileGrabber::scratchChanged(bool & _b)
//    {
//        if(_b)
//        {
//            ofVideoPlayer::setPaused(_b);
//        }
//        else
//        {
//            ofVideoPlayer::setPaused(_b);
//        }
//    }
//
//    //-----------------------------------------------------------------------------------
//    void ImageFileGrabber::headerChanged(float & _f)
//    {
//        if(paramScratch) ofVideoPlayer::setPosition(_f);
//    }
}
