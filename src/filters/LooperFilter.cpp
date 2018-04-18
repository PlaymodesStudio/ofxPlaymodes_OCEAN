
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
        loopStartedAtMs=0.0;
        loopDurationMsWhenTriggered=0.0;
        BPMfactor=1.0;
        restart=false;
        
        parameters->add(paramFrameIn.set("Frame In", myFrame));
        parameters->add(paramDoLoop.set("Loop", false));
        parameters->add(paramRestart.set("Restart"));
        parameters->add(paramCapturedTimeBeatDiv.set("Time Div",1,1,32));
        parameters->add(paramCapturedTimeBeatMult.set("Time Mult",1,1,32));
        parameters->add(paramGatePct.set("Gate",0.0,0.0,1.0));
        parameters->add(paramOffsetMs.set("Offset Ms",0.0,0.0,5000.0));
        parameters->add(paramFrameOut.set("Frame Output", myFrame));


        paramRestart.addListener(this,&LooperFilter::doRestart);
        paramDoLoop.addListener(this, &LooperFilter::doLoopChanged);
        paramCapturedTimeBeatMult.addListener(this, &LooperFilter::loopTimeChanged);
        paramCapturedTimeBeatDiv.addListener(this, &LooperFilter::loopTimeChanged);
        paramFrameIn.addListener(this, &LooperFilter::newVideoFrame);
        
    }
    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* LooperFilter::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoFrame>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoFrame>(), sink.cast<ofxPm::VideoFrame>());
        }
        return nullptr;
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
            double elapsedSincePressedLoop = ofGetElapsedTimeMillis()-loopStartedAtMs;
            //cout << "LooperFilter:: elapsedSincrePressedLoop : " << elapsedSincePressedLoop << " // Loop Started : " << loopStarted << " // Loop Duration " << loopDurationMs << endl;
            
            float timeOffsetOnLoop = 0.0;
            
            if(!paramDoLoop)
            {
                // NO LOOP
                // not looping, so just feeding the buffer with the incoming frame.
                buffer.newVideoFrame(_frame);
                paramFrameOut = _frame;
            }
            else if((paramDoLoop && (elapsedSincePressedLoop < loopDurationMs)))
            {
                // RECORDING LOOP
                // We've pressed "loop" , so capturing into buffer enable and we need to let the time of loop pass before stoping the capture
                
                buffer.newVideoFrame(_frame);
                paramFrameOut = _frame;
                _phasor.resetPhasor();
                timeOffsetOnLoop = elapsedSincePressedLoop - loopDurationMs;
                cout << "LooperFilter::capturing loooop : " << timeOffsetOnLoop  << endl;
            }
            else
            {
                // PLAYING LOOP
                timeOffsetOnLoop = elapsedSincePressedLoop - loopDurationMs;
                //cout << "LooperFilter::PLAYING loooop with offset: " << timeOffsetOnLoop  << endl;
                // Loop already captured, relooping
//                float f = ofGetElapsedTimeMillis()-loopStartedAtMs;
//                float loopDelayMs = loopDurationMsWhenTriggered - fmod(f,loopDurationMs);
                //setting the delay of the header that we will put as frame out .
                // ^^PHASOR^^
                
                float phase = 1.0-_phasor.getPhasor();
                float loopDelayMs = phase * loopDurationMs;
                
                //cout << "LOOPING: DurationMs  " << loopDurationMs << " __ Ph : " << phase << " __ Loop Delay : " <<loopDelayMs << endl;
                //cout << "Phasor : " << _phasor.getBpm() << " M : " << _phasor.getBeatsMult() << " D : " << _phasor.getBeatsDiv() << endl;
                videoHeader.setDelayMs(loopDelayMs + paramOffsetMs);
                
//                float loopStart = loopDurationMsWhenTriggered;
//                float loopEnd = loopDurationMsWhenTriggered - loopDurationMs;
//                float whichPctOfLoopDone = ofMap(loopDelayMs,loopStart,loopEnd,0.0,1.0);

//                cout << endl;
//                cout << " ............................. " << endl;
//                cout << "LoopDelayMs " << loopDelayMs << " // LoopDurationMs : " << loopDurationMs << " // whichPctOfLoopDone : " << whichPctOfLoopDone << " >?? " << (1.0-paramGatePct) << endl;
//                cout << " ____ BPMFactor :  " << BPMfactor << " // paramGatePct :  "  << paramGatePct << " // loopDurationWhenTriggered : " << loopDurationMsWhenTriggered <<  endl ;

                /*
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
                 */
                paramFrameOut = videoHeader.getNextVideoFrame();
            }
                 
        }
    }

    //-----------------------------------------
    void LooperFilter::loopTimeChanged(int &_i)
    {

        //        float gBPM = parametersControl::getInstance().getGlobalBPM();
        float gBPM = 120.0;
        _phasor.setBeatDiv(paramCapturedTimeBeatDiv);
        _phasor.setBeatMult(paramCapturedTimeBeatMult);
        
        cout << paramCapturedTimeBeatDiv << endl;
        if(paramCapturedTimeBeatDiv!=0)
        {
            BPMfactor = (float(paramCapturedTimeBeatMult)/float(paramCapturedTimeBeatDiv));
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
        
        _phasor.resetPhasor();
        cout << "LooperFilter::PRESSED LOOP !! " << endl;
        loopStartedAtMs = ofGetElapsedTimeMillis();
        
    }
    //-----------------------------------------
    void LooperFilter::doRestart()
    {
        cout << "LooperFilter::Restarting!!" << endl;
        restart = true;
        _phasor.resetPhasor();
    }
    
}

