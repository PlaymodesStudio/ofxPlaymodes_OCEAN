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
    
    void MirrorRenderer::setup(){
        parameters->add(paramFrameIn.set("Frame In", VideoFrame()));
        parameters->add(mirrorSize.set("Size", 1, 1, 10));
        parameters->add(spacing.set("Spacing", 1, 0, 2));
        parameters->add(mirrorInvert.set("Invert", false));
        parameters->add(paramFrameOut.set("Frame Out", VideoFrame()));
        
        fbo.allocate(1920, 1200);
        
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
                    int step = 1920/((mirrorSize*2)+1);
                    if(i < mirrorSize){
                        int distance = step*(i+1);
                        int spacingAmount = ((1920/2)-distance) * spacing;
                        ofTranslate((1920/2)-spacingAmount , 0);
                    }else{
                        int distance = step*(i+1);
                        int spacingAmount = (distance-(1920/2)) * spacing;
                        ofTranslate(spacingAmount + (1920/2) , 0);
                    }
                    ofScale((i%2 == mirrorInvert) ? -1200.0/1280.0 : 1200.0/1280.0, 1200.0/1280.0);
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
