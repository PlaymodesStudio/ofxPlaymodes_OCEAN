
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
        buffer.clear();
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
        parameters->add(paramBufferSize.set("Buffer Size",240,1,2000));
        addParameterToGroupAndInfo(paramDoPlay.set("PLAY", false));//.isSavePreset = false;
        addParameterToGroupAndInfo(paramShowOnRec.set("Show On Rec", true));//.isSavePreset = false;
        addParameterToGroupAndInfo(paramDoRefresh.set("Refresh", false));//.isSavePreset = false;
        parameters->add(paramRefreshLoopAt.set("Refresh Loop At",4,2,32));
        parameters->add(paramRestart.set("Restart"));
        parameters->add(paramCapturedTimeBeatDiv.set("Time Div",1,1,32));
        parameters->add(paramCapturedTimeBeatMult.set("Time Mult",1,1,32));
        addParameterToGroupAndInfo(paramGatePct.set("Gate",0.0,0.0,1.0));
        parameters->add(paramSpeedBoost.set("Speed Boost",1.0,-4.0,4.0));
        parameters->add(paramOffsetMs.set("Offset",0.0,0.0,4000.0));
        addParameterToGroupAndInfo(paramDoLoop.set("Loop", false));//.isSavePreset = false;
        addParameterToGroupAndInfo(paramDoRec.set("Rec", true));//.isSavePreset = false;
        addParameterToGroupAndInfo(paramPhasorOut.set("Phasor Out",0.0,0.0,1.0));//.isSavePreset = false;
        parameters->add(paramFrameOut.set("Frame Output", myFrame));

        listeners.push(paramRestart.newListener(this,&LooperFilter::doRestart));
        listeners.push(paramDoLoop.newListener(this, &LooperFilter::doLoopChanged));
        listeners.push(paramDoRec.newListener(this, &LooperFilter::doRecChanged));
        listeners.push(paramCapturedTimeBeatMult.newListener(this, &LooperFilter::loopTimeChanged));
        listeners.push(paramCapturedTimeBeatDiv.newListener(this, &LooperFilter::loopTimeChanged));
        listeners.push(paramFrameIn.newListener(this, &LooperFilter::newVideoFrame));
        listeners.push(paramBufferSize.newListener(this, &LooperFilter::bufferSizeChanged));
        listeners.push(paramDoPlay.newListener(this,&LooperFilter::playChanged));
        listeners.push(paramRefreshLoopAt.newListener(this,&LooperFilter::refreshAtChanged));
        listeners.push(paramDoRefresh.newListener(this,&LooperFilter::doRefreshChanged));
        
        
       listeners.push(_phasor.phasorCycle.newListener(this, &LooperFilter::phasorCycleEvent));
        
        
        int k=0;
        loopTimeChanged(k);
        buffer.setupNodeBased();
        buffer.setBufferSize(paramBufferSize);
        
    }
    //--------------------------------------------------------
    VideoFrame LooperFilter::getNextVideoFrame()
    {
        return myFrame;
    }
    //--------------------------------------------------------
    void LooperFilter::update(ofEventArgs &e)
    {
        paramPhasorOut = _phasor.getPhasor();
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
            
            if(paramDoRec)
            {
                // RECORDING !!
                // feeding the buffer with the incoming frame.
                buffer.newVideoFrame(_frame);
                
                if(!paramShowOnRec)
                {
                    // send to output a new frame (blank)
                    vfAux = VideoFrame();
                }
            }
            else if(paramDoLoop)
            {
                // PLAYING LOOP
                // Loop already captured, relooping
                
                phase = 1.0-fmod(_phasor.getPhasor()*paramSpeedBoost,1.0);
                // added - 1 frame to avoid flickr on Refresh !
                float fps=60.0;
                float loopDelayMs = phase * (loopDurationMs - ((2.0/fps)*1000.0));
                float offsetInMs = (paramOffsetMs);//: * loopDurationMs);
                
                
                videoHeader.setDelayMs(loopDelayMs + offsetInMs);// - ((2.0/fps)*1000.0));
                //cout << "Looper: Looping at phase : " << phase << " --> " <<  loopDelayMs + offsetInMs << endl;

                float loopEnd = videoHeader.getDelayMs() + loopDurationMs;
                float loopStart = loopEnd+loopDurationMs;
                float whichPctOfLoopDone = ofMap(loopDelayMs,loopStart,loopEnd,0.0,1.0);

                vfAux = videoHeader.getNextVideoFrame();

            }

            
            // Gate management
            float ph = _phasor.getPhasor();
            if(paramDoLoop && ((ph)> (1.0 - paramGatePct)))
            {
                // we need to set to black, we're GATING !!
                paramFrameOut = VideoFrame();
            }
            else
            {
                // send the vfAux that we've been cooking ...
                paramFrameOut = vfAux;
            }
            
        }
    }
    //-----------------------------------------
    void LooperFilter::bufferSizeChanged(int &_i)
    {
        //maxSize = i;
    }
    //-----------------------------------------
    void LooperFilter::loopTimeChanged(int &_i)
    {

        //        float gBPM = parametersControl::getInstance().getGlobalBPM();
        float gBPM = 120.0;
        _phasor.setBeatsDiv(paramCapturedTimeBeatDiv);
        _phasor.setBeatsMult(paramCapturedTimeBeatMult);
        
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
        //_phasor.resetPhasor();
    }
    //-----------------------------------------
    void LooperFilter::doRecChanged(bool& _b)
    {
        buffer.changedIsRecording(_b);
        //cout << "Looper: Changing buffer to record to : " << _b << endl;
    }
    //-----------------------------------------
    void LooperFilter::doRestart()
    {
        restart = true;
        _phasor.resetPhasor();
    }
    //-----------------------------------------
    void LooperFilter::doRefreshChanged(bool& _b)
    {
        if(!_b)
        {
            phasorNumCycles=0;
            paramDoPlay = false;
            paramDoLoop = false;
            paramDoRec = true;

        }
        else
        {
            paramDoPlay = true;
        }

    }

    //-----------------------------------------
    void LooperFilter::phasorCycleEvent()
    {
        if(paramDoPlay&&paramDoRefresh)
        {
            if(paramRefreshLoopAt>0)
            {
                if(phasorNumCycles==0)
                {
                    // first cycle, record !
                    paramDoLoop = false;
                    paramDoRec = true;
                    //phasorNumCycles=1;
                }
                else if (phasorNumCycles==1)
                {
                    // set To Play the recorded loop
                    paramDoRec = false;
                    paramDoLoop = true;
                    //phasorNumCycles++;

                }
                
                if(phasorNumCycles>=paramRefreshLoopAt-1)
                {
                    phasorNumCycles=0;
                }
                else
                {
                    phasorNumCycles++;
                }

                
            }
        }
        else phasorNumCycles = 0;
//        cout << ">>><<<<<<<<<<<<<<<<<> Reset phasor  <<<<<<<<<< Cycles :" << phasorNumCycles << endl;
    }

    
    //-----------------------------------------
    void LooperFilter::playChanged(bool &b)
    {
        if(!paramDoRefresh)
        {
            if(b)
            {
                // if PLAY
                _phasor.resetPhasor();
                paramDoRec=false;
                paramDoLoop=true;
            }
            else
            {
                // if NOT PLAY
                paramDoRec=true;
                paramDoLoop=false;
            }
        }

    }
    
    //-----------------------------------------
    void LooperFilter::refreshAtChanged(int &i)
    {
        //if(i!=0) _phasor.resetPhasor();
        if(i==0)
        {
            _phasor.resetPhasor();
            paramDoRec=true;
            paramDoLoop=false;
            phasorNumCycles = 0;
        }
        else
        {
            phasorNumCycles = 0;
        }
    }

    
}


