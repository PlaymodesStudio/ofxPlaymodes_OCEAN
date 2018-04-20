//
//#include "VideoGrabberDeckLinkNodeBased.h"
//
//namespace ofxPm{
//    VideoGrabberDeckLinkNodeBased::VideoGrabberDeckLinkNodeBased(): ofxOceanodeNodeModel("Video Grabber")
//    {
//        //numGrabberDevices = VideoGrabberNodeBased::listDevices().size();
//        // TODO : this is provisional !!
//        numGrabberDevices = 2;
//        
//        parameters->add(paramDeviceId.set("DeviceId", 0, 0, numGrabberDevices-1));
//        parameters->add(paramGrab.set("Grab",true));
//        parameters->add(paramConnect.set("Connect"));
//        parameters->add(paramResolutionX.set("Resolution X",640,0,1920));
//        parameters->add(paramResolutionY.set("Resolution Y",480,0,1080));
//        parameters->add(paramFps.set("FPS",60,0,60));
//        parameters->add(paramFrameOut.set("Frame Output", frame));
//        
//        paramConnect.addListener(this, &VideoGrabberNodeBased::connectToDevice);
//        
//    }
//{
//}
////------------------------------------------------------
//VideoGrabberDeckLinkNodeBased::~VideoGrabberDeckLinkNodeBased()
//{
//}
////------------------------------------------------------
//bool VideoGrabberDeckLinkNodeBased::initGrabber(int w, int h)
//{
//    bool ret;
//    
//    if (input.setup(0)) {
//        input.start(bmdModeHD1080p6000);
//        ret = true;
//    }
//    else
//    {
//        ret = false;
//    }
//
//    frame    = VideoFrame::newVideoFrame(getPixelsRef());
//    VideoSource::width    = w;
//    VideoSource::height   = h;
//    
//    fbo.allocate(w,h,GL_RGB);
//    
//    printf("VideoGrabberDeckLink::initGrabber %d %d\n",VideoSource::width,VideoSource::height);
//    return ret;
//}
//
////------------------------------------------------------
//    VideoFrame VideoGrabberDeckLinkNodeBased::getNextVideoFrame(){
//    return frame;
//}
//
////------------------------------------------------------
//    void VideoGrabberDeckLinkNodeBased::update(ofEventArgs &e)
//{
//    if(paramGrab)
//    {
//        input.update();
//        
//        fbo.begin();
//        {
//            ofSetColor(255);
//            input.draw(0,0,VideoSource::width,VideoSource::height);
//        }
//        fbo.end();
//        
//        fbo.readToPixels(pixels);
//        
//        newFrame(pixels);
//    }
//}
//
//void VideoGrabberDeckLinkNodeBased::newFrame(ofPixels & pixels){
//    frame = VideoFrame::newVideoFrame(pixels);
//    paramFrameOut = frame;
//}
//
////------------------------------------------------------
//float VideoGrabberDeckLinkNodeBased::getFps(){
//    return fps;
//}
//    
////------------------------------------------------------
//void VideoGrabberDeckLinkNodeBased::setFps(float fps){
//    this->fps = fps;
//}
//
//    
//}
