
#include "MultixFilter.h"

namespace ofxPm{

    //------------------------------------------------------------------
    MultixFilter::MultixFilter(): ofxOceanodeNodeModel("Multix FX")
    {
        setupNodeBased();
    }
    //------------------------------------------------------------------
    MultixFilter::~MultixFilter()
    {
        fbo.clear();
        multixDelaysInMs.clear();
    }

    //------------------------------------------------------------------
    void MultixFilter::setupNodeBased()
    {
        autoBPM=false;
        color = ofColor::white;
        oldBufferSize=0;
        desiredBufferSize=900;
        
        parameters->add(paramVideoBufferInput.set("Buffer Input", nullptr));
        parameters->add(paramNumHeaders.set("Num Headers",1,1,900));
        //parameters->add(paramOpacityMode.set("Opacity Mode",0,0,2));
        parameters->add(paramMinMaxBlend.set("MinMax Blend",true));
        parameters->add(paramUseBPM.set("Use BPM",false));
        parameters->add(paramOffsetBeatDiv.set("Beats Div",1,1,32));
        parameters->add(paramOffsetBeatMult.set("Beats Mult",1,1,32));
        parameters->add(paramManualOffsetMs.set("Manual Offset Ms",{33.0},{0.0},{7500}));
        parameters->add(paramOpacityVector.set("Opacity Vector",{1},{0},{1}));
        //parameters->add(paramLinearDistribution.set("Linear Distribution",true));
        //parameters->add(paramDistributionVector.set("Distribution Vector",{0},{0},{1}));
        parameters->add(paramOversize.set("Oversize",0,0,1));
        
        parameters->add(paramFrameOut.set("Frame Output", frame));

        listeners.push(paramOffsetBeatDiv.newListener(this, &MultixFilter::recalculate));
        listeners.push(paramOffsetBeatMult.newListener(this, &MultixFilter::recalculate));
        listeners.push(paramNumHeaders.newListener(this,&MultixFilter::recalculate));
        listeners.push(paramManualOffsetMs.newListener(this,&MultixFilter::changedManualOffsetMs));
        listeners.push(paramVideoBufferInput.newListener(this, &MultixFilter::changedVideoBuffer));
        //listeners.push(paramDistributionVector.newListener(this,&MultixFilter::changedDistributionVector));
        
        int i=0;
        recalculate(i);

    }
    //------------------------------------------------------------
    void MultixFilter::update(ofEventArgs &e)
    {
    }

    //------------------------------------------------------------------
    void MultixFilter::changedManualOffsetMs(vector<float> &f)
    {
        int i=0;
        recalculate(i);
    }
    
    //-----------------------------------------
    void ofxPm::MultixFilter::recalculate(int &_i)
    {
        vector<float> vf;
        float oneCopyMs;
        if(paramUseBPM)
        {
            float BPMfactor;
            if(paramOffsetBeatDiv!=0)
            {
                BPMfactor = (float(paramOffsetBeatMult)/float(paramOffsetBeatDiv));
            }
            else  BPMfactor = 1.0;

            float oneBeatMs = (60.0/myBPM)*1000;
            oneCopyMs = oneBeatMs / BPMfactor;
            if( oneCopyMs != paramManualOffsetMs->at(0)) paramManualOffsetMs = vector<float>(1, oneCopyMs);
        }
        
        else if(!paramUseBPM)
        {
            //oneCopyMs = paramManualOffsetMs;
        }
        //this->setNumHeaders(paramNumHeaders);
    
        for(int i=0;i<paramNumHeaders;i++)
        {
            if(true) // not playing with distribution now ...
            {
                // in linear distribution, the copies are spaced equally a time/distance defined by BPM and beatMult/Div
                if(paramManualOffsetMs->size() != paramNumHeaders){
                    vf.push_back(i*paramManualOffsetMs->at(0));
                }else{
                    vf.push_back(paramManualOffsetMs->at(i));
                }
                //cout << "Multix :: Recalculating i : " << i << "Delay :  " << i*oneCopyMs  << endl;
            }
            else
            {
                /*
                float headerPct = float(i) / float(paramNumHeaders);
                
                int oscillatorReIndex = int(headerPct * paramDistributionVector.get().size());
                
                
//                int oscillatorReIndex = int(ofMap(i,0,float(paramNumHeaders),0,float(numOscillatorBanks)));
                float m=0.0;
                mutex.lock();
                {
                    int vecSiz = paramDistributionVector.get().size();
                    if(oscillatorReIndex<(vecSiz))
                    {
                        m = paramDistributionVector.get().at(oscillatorReIndex);
                    }
                    vf.push_back(ofMap(m,0.0,1.0, 0.0, oneCopyMs));
                    //vf.push_back(ofMap(guiMultixValues.get()[oscillatorReIndex],0.0,1.0, 0.0, oneCopyMs));
                }
                mutex.unlock();
               //cout << i << " ::  OscillatorReindex: " << oscillatorReIndex << " // m : " << m << " :: maxSize : " << paramVideoBufferInput.get()->getMaxSize()<< endl;
                 */
            }
        }
            this->updateValuesMs(vf);

        //    // Calculating "Overflow" it happens when we're trying to fetch a video frame that is out of bounds in the buffer
        //    float oneFrameMs = (1.0 / grabFPS) * 1000.0;
        //    float timeInBuffer = vBuffer.getMaxSize()*oneFrameMs;
        //    float timeInCopies = oneCopyMs*guiNumCopies;
        //    if(timeInCopies > timeInBuffer)
        //    {
        //        cout << "OVERFLOOOOW!! OneCopyMs : " << oneCopyMs << " // guiNumCopies : " << guiNumCopies << " = " << timeInCopies << " //  > " << timeInBuffer << " // buffer Size : " << vBuffer.getMaxSize() << endl;
        //        copiesOverflowBuffer = true;
        //    }
        //    else
        //    {
        //        copiesOverflowBuffer = false;
        //    }
        
    }
    

    //--------------------------------------------------------
    void MultixFilter::updateValuesMs(vector<float> _vf)
    {
//        if(paramVideoBufferInput.get()!=NULL)
        if(true)
        {
            multixDelaysInMs.resize(paramNumHeaders);
            
            // update the delayMs for each
            for(int i=multixDelaysInMs.size()-1; i>=0; i--)
            {
                multixDelaysInMs[i] = _vf[i];
            }
            
        }

    }
    
    
//--------------------------------------------------------
VideoFrame MultixFilter::getNextVideoFrame()
{
    //    if(source->getNextVideoFrame()!=NULL)
    //    {
    //        return source->getNextVideoFrame();
    //    }
    //newVideoFrame(frame);
    return frame;
}

//--------------------------------------------------------
void MultixFilter::newVideoFrame(VideoFrame & _frame)
{

    if(paramNumHeaders>0)
    {
        fbo.begin();
        {
            if(paramMinMaxBlend) ofClear(0,0,0,255);
            else ofClear(255,255,255,255);
            
            ofSetColor(255);
            drawIntoFbo(0,0,fbo.getWidth(),fbo.getHeight());
        }
        fbo.end();
        
        frame = VideoFrame::newVideoFrame(fbo);
        
        paramFrameOut = frame;
    }
}

//--------------------------------------------------------
void MultixFilter::drawIntoFbo(int x, int y,int w, int h)
{
    // TO DO : needed ?
    ofEnableAlphaBlending();

    if(paramMinMaxBlend)
    {
        glBlendEquationEXT(GL_MAX);
    }else{
        glBlendEquationEXT(GL_MIN);
    }

    int headersInAction = 0;
    float oneFrameMs	= 1000.0 / paramVideoBufferInput.get()->getFps();
    float totalBufferSizeInMs = paramVideoBufferInput.get()->getBufferSize() * oneFrameMs;

    float opac = 1.0;
    bool anyOversize=false;
	for(int i = paramNumHeaders-1; i>=0; i--)
    {
        // if delay time of each videoRenderer is in the right range of Ms (0..TotalMs)
        /*
        switch(paramOpacityMode)
        {
            case 0 :
                opac = 1.0;
                break;
            case 1 :
                opac = (1.0/paramNumHeaders) * (paramNumHeaders-i);
                break;
            case 2 :
                opac = 1.0-(1.0/paramNumHeaders) * (paramNumHeaders-i-1);
                break;

        }
         */
        
        // OPACITY
        if((i < paramOpacityVector.get().size()) && (paramOpacityVector.get().size()>1) )
        {
            opac = paramOpacityVector.get().at(i);
        }
        else
        {
            opac = 1.0;
        }

        //cout << "Multix  : i : " << i << " Opac = " << opac << " Size of VecOpac = " << paramOpacityVector.get().size() << endl;
        
        
        if((multixDelaysInMs[i]>=0)&&(multixDelaysInMs[i] < totalBufferSizeInMs))
        {
            headersInAction++;
            ofSetColor((opac*255.0),255.0);
            videoHeader.setDelayMs(multixDelaysInMs[i]);
            VideoFrame vf = videoHeader.getNextVideoFrame();
            
            if(!vf.isNull())
            {
                vf.getTextureRef().draw(x,y,w,h);
                //cout << "Multix : Drawing Texture !!" << endl;
            }
            else
            {
                cout << "Multix :: VF is null!! " << endl;
            }
            //paramOversize =0;
        }
        else
        {
            //cout << "MultixFilter:: Out of time in Ms range !! Copy : " << i << "Delay in Ms = " << multixDelaysInMs[i] << " Max Buffer Size : " << paramVideoBufferInput.get()->getBufferSize() << endl;
            anyOversize=true;
            
        }
	}
    if(anyOversize) paramOversize = 1;
    else paramOversize = 0;
    ofDisableAlphaBlending();

    
//    // Draw Multix Shit
////    int copyWidth = 640.0 / float(videoBuffer->getMaxSize());
//    int copyWidth = 640.0 / float(videoHeader.size());
//    for(int i=0;i<videoHeader.size();i++)
//    {
//        ofVec2f origin = ofVec2f(copyWidth*i+5,500);
//        ofSetColor(255,0,0);
//        ofDrawRectangle(ofPoint(origin), (copyWidth), 100.0 * (videoHeader[i].getDelayMs()/1000.0));
//    }
//    ofSetColor(255,0,0);
//    //float oneFrameMs = (1.0 / 60.0) * 1000.0;
//    float totalLengthInMs = videoBuffer->getMaxSize() * oneFrameMs;
//    int maxHeight = 100 * (totalLengthInMs/1000.0);
//    ofDrawLine(0,500 + maxHeight , 640, 500 + maxHeight);
    
    // TO DO : needed ?
    glBlendEquationEXT(GL_ADD);
    
}

 
    //-----------------------------------------
    void MultixFilter::changedVideoBuffer(ofxPm::VideoBufferNodeBased* &_videoBuffer)
    {
        if(_videoBuffer!=NULL)
        {
            // allocate fbo where to draw
            if ((fbo.getWidth()<=0) && (paramVideoBufferInput.get()->getWidth()) )
            {
                // setup FBO
                int resX = paramVideoBufferInput.get()->getWidth();
                int resY = paramVideoBufferInput.get()->getHeight();
                fbo.allocate(resX,resY,GL_RGB);
                
                // setup Headers
                videoHeader.setup(paramVideoBufferInput.get());
                videoHeader.setDelayMs(0.0);
                
            }
        
            ofxPm::VideoFrame vf;
            newVideoFrame(vf);
        }
    }
}

