
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
        loopDurationMsWhenTriggered=0.0;
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
        
        //TODO : is this the best way to configure header and buffer ?
        if(buffNumFrames==0)
        {
            cout << "LooperFilter::Buffer is 0 frame long..." << endl;
        }
        else
        {
            videoHeader.setup(&buffer);
        }
        
        if(!frameIsNull)
        {
            double elapsedSincePressedLoop = ofGetElapsedTimeMillis()-loopStarted;
            //cout << "LooperFilter:: elapsedSincrePressedLoop : " << elapsedSincePressedLoop << " // Loop Started : " << loopStarted << " // Loop Duration " << loopDurationMs << endl;
            if((!paramDoLoop) || (paramDoLoop && (elapsedSincePressedLoop < loopDurationMs)))
            {
                // We've pressed "loop" , so capturing into buffer enable and we need to let the time of loop pass before stoping the capture
                
                buffer.newVideoFrame(_frame);
                paramFrameOut = _frame;
                //cout << "LooperFilter::capturing loooop" << endl;
            }
            else
            {
                // Loop already captured, relooping 
                float loopStart = loopDurationMsWhenTriggered;
                float loopEnd = loopDurationMsWhenTriggered - loopDurationMs;
                float loopDelayMs = loopDurationMsWhenTriggered - fmod(ofGetElapsedTimeMillis()-loopStarted,loopDurationMs);
                videoHeader.setDelayMs(loopDelayMs);
                
                float whichPctOfLoopDone = ofMap(loopDelayMs,loopStart,loopEnd,0.0,1.0);

//                cout << endl;
//                cout << " ............................. " << endl;
//                cout << "LoopDelayMs " << loopDelayMs << " // LoopDurationMs : " << loopDurationMs << " // whichPctOfLoopDone : " << whichPctOfLoopDone << " >?? " << (1.0-paramGatePct) << endl;
//                cout << " ____ BPMFactor :  " << BPMfactor << " // paramGatePct :  "  << paramGatePct << " // loopDurationWhenTriggered : " << loopDurationMsWhenTriggered <<  endl ;

                // Gate management
                if(whichPctOfLoopDone > (1.0 - paramGatePct))
                {
                    // we need to set to black, we're GATING !!
                    ofxPm::VideoFrame vfAux;
                    paramFrameOut = vfAux;
//                    cout << "LooperFilter::BLACK of Gate!!" <<endl;
                }
                else
                {
                    paramFrameOut = videoHeader.getNextVideoFrame();
                }
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
        loopDurationMs = oneBeatMs / BPMfactor;
        cout << "LooperFilter:: Changed Loop Time Duration = " << loopDurationMs << " Ms !!!!!!!!!!!!!!! " <<  endl;
    }
    //-----------------------------------------
    void LooperFilter::doLoopChanged(bool& _b)
    {
        int i=0;
        loopTimeChanged(i);
        loopDurationMsWhenTriggered = loopDurationMs;

        loopStarted = ofGetElapsedTimeMillis();
        
        cout << "LooperFilter::PRESSED LOOP !! starting at :  " << loopStarted << endl;

    }

    
}

