//
//  MirrorRenderer.cpp
//  VJYourself_OCEAN
//
//  Created by Playmodes Studio on 05/04/2019.
//

#include "MirrorRenderer.h"

namespace ofxPm
{
    MirrorRenderer::MirrorRenderer() : ofxOceanodeNodeModel("Mirror Renderer"){
        
    }
    
int resX = 1920;
int resY = 1200;

    void MirrorRenderer::setup(){
        addParameter(paramFrameIn.set("Frame In", VideoFrame()));
        addParameter(mirrorSize.set("Size", 1, 1, 10));
        addParameter(spacing.set("Spacing", 1, 0, 2));
        addParameter(scale.set("Scale", 1, 0, 2));
        addParameter(mirrorInvert.set("Invert", false));
        addParameter(paramFrameOut.set("Frame Out", VideoFrame()));
        
        fbo.allocate(resX, resY);
        
        listeners.push(paramFrameIn.newListener([this](VideoFrame & frame){
            if(!frame.isNull()){
                int w = frame.getWidth();
                int h = frame.getHeight();
                fbo.begin();
                ofClear(0,0,0, 255);
                ofEnableAlphaBlending();
                
                if(true){
                    glBlendEquationEXT(GL_MAX);
                }else{
                    glBlendEquationEXT(GL_MIN);
                }
                for(int i = 0; i < mirrorSize*2; i++){
                    ofPushMatrix();
                    float step = resX/((mirrorSize*2)+1);
                    if(i < mirrorSize){
                        float distance = step*(i+1);
                        float spacingAmount = ((resX/2.0f)-distance) * spacing;
                        ofTranslate((resX/2.0f)-spacingAmount , fbo.getHeight()*(1-scale));
                    }else{
                        float distance = step*(i+1);
                        float spacingAmount = (distance-(resX/2.0f)) * spacing;
                        ofTranslate(spacingAmount + (resX/2.0f) , fbo.getHeight()*(1-scale));
                    }
                    ofScale((i%2 == mirrorInvert) ? -resY/1280.0 : resY/1280.0, resY/1280.0);
                    ofScale(scale, scale);
                    ofTranslate(-(w/2), 0);
                    
                    frame.getTextureRef().draw(0,0);
                    ofPopMatrix();
                }
                ofDisableAlphaBlending();
                fbo.end();
                paramFrameOut = VideoFrame::newVideoFrame(fbo);
            }
        }));
    }
    
    
}
