
#include "LooperFilter.h"
#include "ofxOceanodeContainer.h"

namespace ofxPm{

    //------------------------------------------------------------------
    LooperFilter::LooperFilter():
        ofxOceanodeNodeModel("Looper FX")
    {
        setupNodeBased();
    }
    //------------------------------------------------------------------
    LooperFilter::~LooperFilter()
    {
    }
    //------------------------------------------------------------------
    void LooperFilter::setupNodeBased()
    {
        int i=1;
        loopTimeChanged(i);
        loopStarted=0.0;
        loopDurationWhenTriggered=0.0;
        BPMfactor=1.0;
        
        parameters->add(paramFrameIn.set("Frame In", myFrame));
        parameters->add(paramDoLoop.set("Loop", false));
        parameters->add(paramOffsetBeatDiv.set("Beats Div",1,1,32));
        parameters->add(paramOffsetBeatMult.set("Beats Mult",1,1,32));
        parameters->add(paramGatePct.set("Gate",0.0,0.0,1.0));
        parameters->add(paramFrameOut.set("Frame Output", myFrame));

        paramDoLoop.addListener(this, &LooperFilter::doLoopChanged);
        paramOffsetBeatDiv.addListener(this, &LooperFilter::loopTimeChanged);
        paramOffsetBeatMult.addListener(this, &LooperFilter::loopTimeChanged);
        paramFrameIn.addListener(this, &LooperFilter::newVideoFrame);
    }
    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* LooperFilter::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoFrame>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoFrame>(), sink.cast<ofxPm::VideoFrame>());
        }
    }
    //--------------------------------------------------------
    VideoFrame LooperFilter::getNextVideoFrame()
    {
        return myFrame;
    }

    //--------------------------------------------------------
    void LooperFilter::newVideoFrame(VideoFrame & _frame)
    {
        bool frameIsNull = _frame.isNull();
        int buffNumFrames = buffer.getSizeInFrames();
        
        if(buffNumFrames==0)
        {
            cout << "Buffer is 0" << endl;
        }
        else
        {
            videoHeader.setup(&buffer);
        }
        
        if(!frameIsNull)
        {
            double elapsedSincePressedLoop = ofGetElapsedTimeMillis()-loopStarted;
//            cout << "LooperFilter::newVideoFrame Buffer Size is : " << buffNumFrames << endl;
            cout << "LooperFilter:: elapsedSincrePressedLoop : " << elapsedSincePressedLoop << " // Loop Started : " << loopStarted << " // Loop Duration " << loopDuration << endl;
            if((!paramDoLoop) || (paramDoLoop && (elapsedSincePressedLoop < loopDuration)))
            {
                //buffer.resume();
                buffer.newVideoFrame(_frame);
                parameters->get("Frame Output").cast<ofxPm::VideoFrame>() = _frame;
                cout << "!!!!!!!!!!! " << endl;
            }
            else
            {
                //buffer.stop();
                float loopDelay = loopDurationWhenTriggered - fmod(ofGetElapsedTimeMillis()-loopStarted,loopDuration);
                videoHeader.setDelayMs(loopDelay);
                

                float whichPctOfLoopDone = 1.0 - (loopDelay / loopDuration);
                cout << "whichPctOfLoopDone : " << whichPctOfLoopDone << endl;
                if(whichPctOfLoopDone < (paramGatePct*BPMfactor))
//                    if(false)
                {
                    ofxPm::VideoFrame vfAux;
                    parameters->get("Frame Output").cast<ofxPm::VideoFrame>() = vfAux;
                }
                else
                {
                    parameters->get("Frame Output").cast<ofxPm::VideoFrame>() = videoHeader.getNextVideoFrame();
                }
                   

                cout << "LoopDelay : " << loopDelay << endl;
            }
        }
    }

    //-----------------------------------------
    void LooperFilter::loopTimeChanged(int &_i)
    {

        //        float gBPM = parametersControl::getInstance().getGlobalBPM();
        float gBPM = 120.0;
        
        if(paramOffsetBeatDiv!=0)
        {
            BPMfactor = (float(paramOffsetBeatMult)/float(paramOffsetBeatDiv));
        }
        else  BPMfactor = 1.0;
        
        float oneBeatMs = (60.0/gBPM)*1000;
        loopDuration = oneBeatMs / BPMfactor;
        cout << "LooperFilter:: Setting Loop Duration = " << loopDuration << endl;
    }
    //-----------------------------------------
    void LooperFilter::doLoopChanged(bool& _b)
    {
        int i=0;
        loopTimeChanged(i);
        loopDurationWhenTriggered = loopDuration;

        loopStarted = ofGetElapsedTimeMillis();
        
        cout << "LooperFilter::PRESSED LOOP !! starting at :  " << loopStarted << endl;

    }

    
}

