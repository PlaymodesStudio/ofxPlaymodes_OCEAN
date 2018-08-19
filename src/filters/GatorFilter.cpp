
#include "GatorFilter.h"

namespace ofxPm{

    //------------------------------------------------------------------
    GatorFilter::GatorFilter():
        ofxOceanodeNodeModel("Gator FX")
    {
        color = ofColor::white;
        setupNodeBased();
    }
    //------------------------------------------------------------------
    GatorFilter::~GatorFilter()
    {
    }
    //------------------------------------------------------------------
    void GatorFilter::setupNodeBased()
    {
        color = ofColor::darkMagenta;

        // create beatMultDiv
        beatMultDiv.push_back(1.0/64.0);
        beatMultDiv.push_back(1.0/32.0);
        beatMultDiv.push_back(1.0/16.0);
        beatMultDiv.push_back(1.0/8.0);
        beatMultDiv.push_back(1.0/4.0);
        beatMultDiv.push_back(1.0/2.0);
        beatMultDiv.push_back(1.0/1.0);
        beatMultDiv.push_back(2.0);
        beatMultDiv.push_back(4.0);
        beatMultDiv.push_back(8.0);
        beatMultDiv.push_back(16.0);
        beatMultDiv.push_back(32.0);
        beatMultDiv.push_back(64.0);
        int numB = beatMultDiv.size();
        doGate = true;
        
        // setup params
        parameters->add(paramFrameIn.set("Frame In", myFrame));
        parameters->add(paramBlackOrWhite.set("Black or White", true));
        parameters->add(paramDoRestart.set("Restart"));
        parameters->add(paramGateFreqBPM.set("Frequency BPM",6,0,numB-1));
        parameters->add(paramBypass.set("Bypass",false));
        parameters->add(paramFrameOut.set("Frame Output", myFrame));

        paramFrameIn.addListener(this, &GatorFilter::newVideoFrame);
        paramGateFreqBPM.addListener(this, &GatorFilter::freqChanged);
        paramDoRestart.addListener(this, &GatorFilter::restartChanged);

        ofAddListener(_phasor.phasorCycle, this, &GatorFilter::phasorCycleEvent);
        
        setFrameResolution(1280, 720);


    }
    //--------------------------------------------------------
    void GatorFilter::setFrameResolution(int _w, int _h)
    {
        cout << "GatorFilter :: Resetting frame resolution to " << _w << " , " << _h << endl;

        // setup black and white
        ofPixels pix;
        frameResolution.x = _w;
        frameResolution.y = _h;
        pix.allocate(frameResolution.x, frameResolution.y,3);
        for(int i=0;i<frameResolution.x*frameResolution.y*3;i++)
        {
            pix[i] = 0;
        }
        blackFrame = VideoFrame(pix);
        for(int i=0;i<frameResolution.x*frameResolution.y*3;i++)
        {
            pix[i] = 255;
        }
        whiteFrame = VideoFrame(pix);

    }

    //--------------------------------------------------------
    VideoFrame GatorFilter::getNextVideoFrame()
    {
        return whiteFrame;
    }

    //--------------------------------------------------------
    void GatorFilter::newVideoFrame(VideoFrame & _frame)
    {
        if(!_frame.isNull())
        {
            if((!doGate)||(paramBypass))
            {
                paramFrameOut = _frame;
            }
            else
            {
                // Let's gate it !!
                
                // check frame size = black & white frames !!
                if((_frame.getWidth()!=frameResolution.x)&&(_frame.getHeight()!=frameResolution.y))
                {
                    setFrameResolution(_frame.getWidth(), _frame.getHeight());
                }
                
                if(paramBlackOrWhite)
                {
                    paramFrameOut = whiteFrame;
                    doGate=false;
                }
                else
                {
                    paramFrameOut = blackFrame ;
                    doGate=false;
                }
            }
        }
    }
    //-----------------------------------------
    void GatorFilter::restartChanged()
    {
        _phasor.resetPhasor();
    }

    //-----------------------------------------
    void GatorFilter::freqChanged(int& _i)
    {
        //reset phasor
        _phasor.resetPhasor();
        
        // calculate beat div/mult
        int half = int(beatMultDiv.size()/2);
        if(_i>=half)
        {
            int m =pow(2,_i-6);
            _phasor.beatsDiv_Param=1;
            _phasor.beatsMult_Param=m;
        }
        else if(_i<half)
        {
            int d =pow(2,6-_i);
            _phasor.beatsDiv_Param=d;
            _phasor.beatsMult_Param=1;
        }
        
        cout << _i << "   ::  DIV : " << _phasor.beatsDiv_Param << " --- MULT : " << _phasor.beatsMult_Param << endl;
    }

//    //-----------------------------------------
//    void GatorFilter::loopTimeChanged(int &_i)
//    {
//
//        //        float gBPM = parametersControl::getInstance().getGlobalBPM();
//        float gBPM = 120.0;
//        _phasor.beatsDiv_Param = paramCapturedTimeBeatDiv;
//        _phasor.beatsMult_Param = paramCapturedTimeBeatMult;
//
//        if(paramCapturedTimeBeatDiv!=0)
//        {
//            BPMfactor = (float(paramCapturedTimeBeatMult)/float(paramCapturedTimeBeatDiv));
//        }
//        else  BPMfactor = 1.0;
//
//        float oneBeatMs = (60.0/myBPM)*1000;
//        loopDurationMs = oneBeatMs / BPMfactor;
//        //cout << "LooperFilter:: Changed Loop Time Duration = " << loopDurationMs << " Ms ! BPM :  "<< myBPM <<  endl;
//    }
    //-----------------------------------------
    void GatorFilter::phasorCycleEvent()
    {
        doGate=true;
    }
    
}


