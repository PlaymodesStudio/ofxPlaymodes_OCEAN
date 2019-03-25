/*
 * VideoFrame.cpp
 *
 *  Created on: 09-oct-2008
 *      Author: arturo castro
 */

#include "VideoFrame.h"
//using Poco::ScopedLock;

namespace ofxPm
{
int VideoFrame::total_num_frames=0;
map<VideoFormat,vector<ofPtr<VideoFrame::Obj> > > VideoFrame::pool;
//ofMutex VideoFrame::poolMutex;
std::mutex VideoFrame::poolMutex;
    
class VideoFrame::Obj{
public:
	Obj()
	:pixelsChanged(false)
	,createdTexPixels(true)
	{
		total_num_frames++;

	}
	Obj(const ofPixels & videoFrame)
	:pixels(videoFrame)
	,pixelsChanged(true)
	,createdTexPixels(true)
	{
    
        //cout << "TNF :" <<  ofGetWindowPtr()->getNSGLContext() << endl;
		total_num_frames++;
        //cout <<"______ Total : "<< total_num_frames << endl;
	}
	Obj(ofTexture & videoFrame)
	:pixelsChanged(false)
	,createdTexPixels(false)
	{
         //cout << "VFT :" <<  ofGetWindowPtr()->getNSGLContext() << endl;
        pixels.allocate(videoFrame.getWidth(),videoFrame.getHeight(),ofGetImageTypeFromGLType(videoFrame.texData.glInternalFormat));
		updateTexture(videoFrame);
		total_num_frames++;
        //cout <<"______"<< total_num_frames << endl;
	}
	Obj(ofFbo & videoFrame)
	:pixelsChanged(false)
	,createdTexPixels(false)
	{
        //cout << "VFF :" <<  ofGetWindowPtr()->getNSGLContext() << endl;
		pixels.allocate(videoFrame.getWidth(),videoFrame.getHeight(),ofGetImageTypeFromGLType(videoFrame.getTextureReference().texData.glInternalFormat));
		updateTexture(videoFrame);
		total_num_frames++;
        //cout <<"______"<< total_num_frames << endl;
	}

	void updateTexture(ofTexture & _tex){
        //cout << "UTX :" <<  ofGetWindowPtr()->getNSGLContext() << endl;
		if(!fbo.isAllocated()){
			fbo.allocate(_tex.getWidth(),_tex.getHeight(),_tex.texData.glInternalFormat);
		}
		fbo.begin();
		_tex.bind();
		ofMesh mesh;
		mesh.setMode(OF_PRIMITIVE_TRIANGLE_FAN);
		mesh.addTexCoord(ofVec2f(0,0));
		mesh.addTexCoord(ofVec2f(1.0,0));
		mesh.addTexCoord(ofVec2f(1.0,1.0));
		mesh.addTexCoord(ofVec2f(0,1.0));
		mesh.addVertex(ofVec3f(0,0));
		mesh.addVertex(ofVec3f(_tex.getWidth(),0));
		mesh.addVertex(ofVec3f(_tex.getWidth(),_tex.getHeight()));
		mesh.addVertex(ofVec3f(0,_tex.getHeight()));

//        mesh.addTexCoord(ofVec2f(0,0));
//        mesh.addTexCoord(ofVec2f(videoFrame.getWidth(),0));
//        mesh.addTexCoord(ofVec2f(videoFrame.getWidth(),videoFrame.getHeight()));
//        mesh.addTexCoord(ofVec2f(0,videoFrame.getHeight()));
//        mesh.addVertex(ofVec3f(0,0));
//        mesh.addVertex(ofVec3f(videoFrame.getWidth(),0));
//        mesh.addVertex(ofVec3f(videoFrame.getWidth(),videoFrame.getHeight()));
//        mesh.addVertex(ofVec3f(0,videoFrame.getHeight()));

        mesh.draw();
		_tex.unbind();
		fbo.end();
	}


	void updateTexture(ofFbo & _fbo){
        //cout << "UPD :" <<  ofGetWindowPtr()->getNSGLContext() << endl;
		if(!fbo.isAllocated()){
			fbo.allocate(_fbo.getWidth(),_fbo.getHeight(),_fbo.getTextureReference().texData.glInternalFormat);
		}
		_fbo.bind();
		glReadBuffer(GL_COLOR_ATTACHMENT0);
		glBindTexture(fbo.getTextureReference().texData.textureTarget, (GLuint)fbo.getTextureReference().texData.textureID);
		glCopyTexImage2D(fbo.getTextureReference().texData.textureTarget,0,fbo.getTextureReference().texData.glInternalFormat,0,0,fbo.getWidth(),fbo.getHeight(),0);
		_fbo.unbind();
		glReadBuffer(GL_BACK);
	}

    ofPixels pixels;
    ofFbo fbo;
    bool pixelsChanged;
    bool createdTexPixels;
};

	VideoFrame::VideoFrame(const ofPixels & videoFrame) 
	:data(new Obj(videoFrame),&VideoFrame::poolDeleter){
	}

	VideoFrame::VideoFrame(ofTexture & videoFrame)
	:data(new Obj(videoFrame),&VideoFrame::poolDeleter){
	}

	VideoFrame::VideoFrame(ofFbo & videoFrame)
	:data(new Obj(videoFrame),&VideoFrame::poolDeleter){
	}


	VideoFrame::VideoFrame()
	{

	}

	VideoFrame::~VideoFrame()
    {
        //cout << "Destroying VideoFrame!!" << endl;
        if(data != nullptr)
        {
            //cout << "Pixels : Alloc : " << data->pixels.isAllocated() << " Size : " << data->pixels.size() << endl;
            
        }
	}

	VideoFrame VideoFrame::newVideoFrame(const ofPixels & _pix){
        //cout << "PIX :" <<  ofGetWindowPtr()->getNSGLContext() << endl;
		VideoFormat format(_pix);
        
		poolMutex.lock();
        
		if(!pool[format].empty()){
			VideoFrame frame;
			//cout << "returning frame from pool" << endl;
			frame.data = pool[format].back();
			pool[format].pop_back();
			poolMutex.unlock();

			frame.refreshTimestamp();
			frame.data->pixels = _pix;
			frame.data->pixelsChanged = true;
			frame.data->createdTexPixels = true;
			return frame;
		}else{
            //cout << "XXXXXXXXXXXXXX __ LEAKING PIXELS __ XXXXXXXXXXXXXXXXXX  " << format.width << " h : " << format.height << " Ch. : " << format.numChannels << endl;
            

			poolMutex.unlock();
			return VideoFrame(_pix);
		}
        
	}

	VideoFrame VideoFrame::newVideoFrame(ofTexture & _tex){
        //cout << "TEX :" <<ofGetWindowPtr()->getNSGLContext() << endl;
		VideoFormat format(_tex);
		poolMutex.lock();
		if(!pool[format].empty()){
			VideoFrame frame;
			//cout << "returning frame from pool" << endl;
			frame.data = pool[format].back();
			pool[format].pop_back();
			poolMutex.unlock();

			frame.refreshTimestamp();
			frame.getFboRef();
			frame.data->updateTexture(_tex);
			frame.data->pixelsChanged = false;
			frame.data->createdTexPixels = false;
			return frame;
		}else{
            //cout << "XXXXXXXXXXXXXX __ LEAKING TEXTURE __ XXXXXXXXXXXXXXXXXX  " << format.width << " h : " << format.height << " Ch. : " << format.numChannels << endl;

			poolMutex.unlock();
			return VideoFrame(_tex);
		}
	}

	VideoFrame VideoFrame::newVideoFrame(ofFbo & videoFrame)
    {
        //cout << "FRM :" <<  ofGetWindowPtr()->getNSGLContext() << endl;
		VideoFormat format(videoFrame);
		
        poolMutex.lock();
        
        
        
        if(!pool[format].empty()){
            //cout << "/////////////////////////////// " << format.width << " h : " << format.height << " Ch. : " << format.numChannels << endl;

			VideoFrame frame;
			//cout << "returning frame from pool" << endl;
			frame.data = pool[format].back();
			pool[format].pop_back();
			poolMutex.unlock();

			frame.refreshTimestamp();
			frame.getFboRef();
			frame.data->updateTexture(videoFrame);
			frame.data->pixelsChanged = false;
			frame.data->createdTexPixels = false;
			return frame;
		}else{
            //cout << "XXXXXXXXXXXXXX __ LEAKING FBO __ XXXXXXXXXXXXXXXXXX  " << format.width << " h : " << format.height << " Ch. : " << format.numChannels << endl;

            poolMutex.unlock();
			return VideoFrame(videoFrame);
		}
	}

	VideoFrame VideoFrame::newVideoFrame(VideoFrame videoFrame){
		if(videoFrame.data->createdTexPixels){
			return newVideoFrame(videoFrame.getPixelsRef());
		}else{
			return newVideoFrame(videoFrame.getFboRef());
		}
	}

	void VideoFrame::poolDeleter(VideoFrame::Obj * obj)
    {
        try {
            std::unique_lock<std::mutex> lock(poolMutex);
            pool[VideoFormat(obj->pixels)].push_back(ofPtr<Obj>(obj,&VideoFrame::poolDeleter));
        }
        catch(const std::exception& e) {
            /* When program terminates, acquiring lock is impossible. */
        }
	}

	ofPixels & VideoFrame::getPixelsRef(){
		return  data->pixels;
	}

	ofTexture & VideoFrame::getTextureRef(){
		if(data->pixelsChanged){
			getFboRef().getTextureReference().loadData(data->pixels);
			data->pixelsChanged = false;
		}
		return getFboRef().getTextureReference();
	}

	ofFbo & VideoFrame::getFboRef(){
		if(!data->fbo.isAllocated()){
			data->fbo.allocate(data->pixels.getWidth(),data->pixels.getHeight(),ofGetGlInternalFormat(data->pixels));
		}
		return data->fbo;
	}

	int VideoFrame::getWidth(){
        ofPixels pix = data->pixels;
		return pix.getWidth();
	}

	int VideoFrame::getHeight(){
        ofPixels pix = data->pixels;
		return data->pixels.getHeight();
	}

	int VideoFrame::getPoolSize(const VideoFormat & format){

        std::unique_lock<std::mutex> lock(poolMutex);
        //poolMutex.lock();
        int res =pool[format].size();
        poolMutex.unlock();

        return res;
	}


	int VideoFrame::getTotalNumFrames(){
		return total_num_frames;
	}

	void VideoFrame::setTextureOnly(bool texOnly){
		data->createdTexPixels=!texOnly;
		data->pixelsChanged=false;
	}
	}
