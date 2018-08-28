
#include "LooperFilter.h"

namespace ofxPm{

    //------------------------------------------------------------------
    LooperFilter::LooperFilter():
        ofxOceanodeNodeModelLocalPreset("Looper FX")
    {
        color = ofColor::orange;
        setupNodeBased();
    }
    //------------------------------------------------------------------
    LooperFilter::~LooperFilter()
    {
    }
    //------------------------------------------------------------------
    void LooperFilter::setupNodeBased()
    {
        color = ofColor::darkMagenta;

        int i=1;
        loopTimeChanged(i);
        BPMfactor=1.0;
        restart=false;
        phasorNumCycles=0;
        paramOffsetMs=0;
        
        parameters->add(paramFrameIn.set("Frame In", myFrame));
        addParameterToGroupAndInfo(paramDoLoop.set("Loop", false)).isSavePreset = false;
        parameters->add(paramRestart.set("Restart"));
        parameters->add(paramCapturedTimeBeatDiv.set("Time Div",1,1,32));
        parameters->add(paramCapturedTimeBeatMult.set("Time Mult",1,1,32));
//        parameters->add(paramGatePct.set("Gate",0.0,0.0,1.0));
        addParameterToGroupAndInfo(paramGatePct.set("Gate",0.0,0.0,1.0)).isSavePreset = false;
        parameters->add(paramSpeedBoost.set("Speed Boost",1.0,-4.0,4.0));
        parameters->add(paramOffsetMs.set("Offset",0.0,0.0,4000.0));
        
        parameters->add(paramFrameOut.set("Frame Output", myFrame));


        paramRestart.addListener(this,&LooperFilter::doRestart);
        paramDoLoop.addListener(this, &LooperFilter::doLoopChanged);
        paramCapturedTimeBeatMult.addListener(this, &LooperFilter::loopTimeChanged);
        paramCapturedTimeBeatDiv.addListener(this, &LooperFilter::loopTimeChanged);
        paramFrameIn.addListener(this, &LooperFilter::newVideoFrame);
        
        ofAddListener(_phasor.phasorCycle, this, &LooperFilter::phasorCycleEvent);
        
        
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
        ofxPm::VideoFrame vfAux;
        float phase=0.0;
        
        //TODO : is this the best way to configure header and buffer ? each frame header.setup(buffer) ???
        if(buffNumFrames==0)
        {
            //cout << "LooperFilter::Buffer is 0 frame long..." << endl;
        }
        else
        {
            videoHeader.setup(&buffer);
        }
        
        if(!frameIsNull)
        {
            // always pass the incoming frame to output... if REC -> to the buffer too
            // if LOOP -> it will overwrite vfAux with the corresponding one
            vfAux = _frame;
            
            if(paramDoLoop)
            {
                // PLAYING LOOP
                // Loop already captured, relooping
                
                phase = 1.0-fmod(_phasor.getPhasor()*paramSpeedBoost,1.0);
                float loopDelayMs = phase * loopDurationMs;
                float offsetInMs = (paramOffsetMs);//: * loopDurationMs);
                
                videoHeader.setDelayMs(loopDelayMs + offsetInMs);
                
                float loopEnd = videoHeader.getDelayMs() + loopDurationMs;
                float loopStart = loopEnd+loopDurationMs;
                float whichPctOfLoopDone = ofMap(loopDelayMs,loopStart,loopEnd,0.0,1.0);

                vfAux = videoHeader.getNextVideoFrame();

            }
            else
            {
                // NOT LOOPING so REC !
                // RECORDING !!
                // feeding the buffer with the incoming frame.
                buffer.newVideoFrame(_frame);
                //cout << "LooperFilter : Doing REC new frame !! Size of buffer is : " << buffer.getSizeInFrames() << endl;
                

            }
            
            // Gate management
            //if(whichPctOfLoopDone > (1.0 - paramGatePct))
            float ph = _phasor.getPhasor();
            if(paramDoLoop && ((ph)> (1.0 - paramGatePct)))
            {
                // we need to set to black, we're GATING !!
//                ofxPm::VideoFrame vfBlack;
                paramFrameOut = myFrame;
            }
            else
            {
                paramFrameOut = vfAux;
            }
        }
    }

    //-----------------------------------------
    void LooperFilter::loopTimeChanged(int &_i)
    {

        //        float gBPM = parametersControl::getInstance().getGlobalBPM();
        float gBPM = 120.0;
        _phasor.beatsDiv_Param = paramCapturedTimeBeatDiv;
        _phasor.beatsMult_Param = paramCapturedTimeBeatMult;
        
        if(paramCapturedTimeBeatDiv!=0)
        {
            BPMfactor = (float(paramCapturedTimeBeatMult)/float(paramCapturedTimeBeatDiv));
        }
        else  BPMfactor = 1.0;
        
        float oneBeatMs = (60.0/myBPM)*1000;
        loopDurationMs = oneBeatMs / BPMfactor;
        //cout << "LooperFilter:: Changed Loop Time Duration = " << loopDurationMs << " Ms ! BPM :  "<< myBPM <<  endl;
    }
    //-----------------------------------------
    void LooperFilter::doLoopChanged(bool& _b)
    {
        int i=0;
        loopTimeChanged(i);
        _phasor.resetPhasor();
        bool b = !_b;
        buffer.changedIsRecording(b);
    }
    //-----------------------------------------
    void LooperFilter::doRecChanged(bool& _b)
    {
// NEED THIS ?
//         _phasor.resetPhasor();
        
    }
    //-----------------------------------------
    void LooperFilter::doRestart()
    {
        cout << "LooperFilter::Restarting!!" << endl;
        restart = true;
        _phasor.resetPhasor();
    }
    //-----------------------------------------
    void LooperFilter::phasorCycleEvent()
    {
//        if(paramDoLoop) phasorNumCycles = phasorNumCycles +1;
//        else phasorNumCycles = 0;
//
//        //cout << "Phasor Cycle ! : "  << phasorNumCycles << endl;
//
//        if((phasorNumCycles>=paramRefreshLoopAt)&&(paramRefreshLoopAt>0))
//        {
//            paramDoLoop = true;
//            bool b = true;
//            doLoopChanged(b);
//            phasorNumCycles = 0;
//        }
    }
    
}


