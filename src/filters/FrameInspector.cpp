
#include "FrameInspector.h"

namespace ofxPm{
    
    //------------------------------------------------------------
    FrameInspector::FrameInspector(): ofxOceanodeNodeModel("Frame Inspector")
    {
        setupNodeBased();
        recordedTimes = 0;
    }

    //------------------------------------------------------------
    FrameInspector::~FrameInspector()
    {
    }

    //------------------------------------------------------------
    void FrameInspector::setupNodeBased()
    {
        color = ofColor::orange;

        lastFrameRendered = ofxPm::VideoFrame();

        parameters = new ofParameterGroup();
        parameters->add(paramFrameIn.set("Frame Input", frame));
        parameters->add(paramFPSNow.set("FPS now",0,0.0,120.0));
        parameters->add(paramFPSAccum.set("FPS Vector",0,0,120));
        parameters->add(paramFrameWidth.set("Frame Width",0,0,2048));
        parameters->add(paramFrameHeight.set("Frame Height",0,0,2048));
        parameters->add(paramFrameOut.set("Frame Output", frame));
        
        listener = paramFrameIn.newListener(this, &FrameInspector::newVideoFrame);
        
        lastTimeMillis = ofGetElapsedTimeMillis();
        recordedTimes++;
    }


    //------------------------------------------------------------
    VideoFrame FrameInspector::getNextVideoFrame()
    {
        return frame;

    }

    //------------------------------------------------------------
    void FrameInspector::newVideoFrame(VideoFrame & _frame)
    {
        //
        paramFPSNow = 1.0 / ((ofGetElapsedTimeMillis() - lastTimeMillis)/1000.0);
        lastTimeMillis = ofGetElapsedTimeMillis();
        
        if(lastTimesVector.size()>500)
        {
            lastTimesVector.pop_front();
        }
        lastTimesVector.push_back(paramFPSNow);
        paramFPSAccum = int(getVectorMean());
        
        bool frameIsNull = _frame.isNull();
        if(!frameIsNull)
        {
            paramFrameOut = _frame;
            paramFrameWidth = _frame.getWidth();
            paramFrameHeight = _frame.getHeight();
        }
         
    }
    //------------------------------------------------------------
    float FrameInspector::getVectorMean()
    {
        float ac=0.0;
        for(int i=0;i<lastTimesVector.size();i++)
        {
            ac = ac + lastTimesVector[i];
        }
        return ac/float(lastTimesVector.size());
    }
}
