/*
 * VideoFormat.h
 *
 *  Created on: 13/02/2012
 *      Author: arturo
 */

#pragma once

namespace ofxPm{
class VideoFormat {
public:
	VideoFormat(int width, int height, int numChannels)
	:width(width)
	,height(height)
	,numChannels(numChannels){}

	VideoFormat(const ofPixels & pixels)
	:width(pixels.getWidth())
	,height(pixels.getHeight())
//    ,numChannels(3){}
    ,numChannels(pixels.getNumChannels()){}

	VideoFormat(ofTexture & tex)
 	:width(tex.getWidth())
	,height(tex.getHeight())
	,numChannels(0)
    {
        switch (tex.getTextureData().glInternalFormat)
        {
            case GL_RGB :
                numChannels = 3;
                break;
            case GL_RGBA :
                numChannels = 4;
                break;
        }
    }
    
    VideoFormat(ofFbo & fbo)
	:width(fbo.getWidth())
	,height(fbo.getHeight())
    ,numChannels(0)
    {
        switch (fbo.getTexture().getTextureData().glInternalFormat)
        {
            case GL_RGB :
                numChannels = 3;
                break;
            case GL_RGBA :
                numChannels = 4;
                break;
        }
    }


	int width, height, numChannels;

	bool operator<(const VideoFormat & f) const{
        //return f.width * f.height * f.numChannels > width * height * numChannels;
        return (f.width * f.height * f.numChannels) + (f.width < f.height ? 1 : 0) > (width * height * numChannels) + (width < height ? 1 : 0);
	}
};
}
