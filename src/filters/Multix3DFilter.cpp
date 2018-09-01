
#include "Multix3DFilter.h"

namespace ofxPm{

    //------------------------------------------------------------------
    Multix3DFilter::Multix3DFilter(): ofxOceanodeNodeModel("Multix3D FX")
    {
        setupNodeBased();
    }
    //------------------------------------------------------------------
    Multix3DFilter::~Multix3DFilter()
    {
        fbo.clear();
        multixDelaysInMs.clear();
    }

    //------------------------------------------------------------------
    void Multix3DFilter::setupNodeBased()
    {
        autoBPM=false;
        color = ofColor::white;

        parameters->add(paramVideoBufferInput.set("Buffer Input", nullptr));
        parameters->add(paramNumHeaders.set("Num Headers",1,1,480));
        parameters->add(paramOpacityMode.set("Opacity Mode",0,0,2));
        parameters->add(paramMinMaxBlend.set("MinMax Blend",true));
        parameters->add(paramUseBPM.set("Use BPM",true));
        parameters->add(paramOffsetBeatDiv.set("Beats Div",1,1,32));
        parameters->add(paramOffsetBeatMult.set("Beats Mult",1,1,32));
        parameters->add(paramManualOffsetMs.set("Manual Offset Ms",33.0,0.0,4000.0));
        parameters->add(paramScale.set("Scale",1.6076,0.0,2.0));
        parameters->add(paramCopiesPositionX.set("Copies Position X",{0},{0},{0}));
        parameters->add(paramCopiesOpacity.set("Copies Opacity",{1.0},{1.0},{1.0}));
        parameters->add(paramLinearDistribution.set("Linear Distribution",true));
        parameters->add(paramDistributionVector.set("Distribution Vector",{0},{0},{1}));
        parameters->add(paramOversize.set("Oversize",0,0,1));
        
        parameters->add(paramFrameOut.set("Frame Output", frame));

        paramOffsetBeatDiv.addListener(this, &Multix3DFilter::recalculate);
        paramOffsetBeatMult.addListener(this, &Multix3DFilter::recalculate);
        paramNumHeaders.addListener(this,&Multix3DFilter::recalculate);
        paramManualOffsetMs.addListener(this,&Multix3DFilter::changedManualOffsetMs);
        paramVideoBufferInput.addListener(this, &Multix3DFilter::changedVideoBuffer);
        paramDistributionVector.addListener(this,&Multix3DFilter::changedDistributionVector);
        paramCopiesPositionX.addListener(this,&::ofxPm::Multix3DFilter::changedCopiesPositionX);

        int i=0;
        recalculate(i);
        
        fbo.allocate(1920,1080);

    }
    //------------------------------------------------------------
    void Multix3DFilter::update(ofEventArgs &e)
    {
    }

    //------------------------------------------------------------------
    void Multix3DFilter::changedCopiesPositionX(vector<float> &_v)
    {
        //cout << "Multix3D : new positons X with size : " << _v.size() << endl;
    }
    
    //------------------------------------------------------------------
    void Multix3DFilter::changedDistributionVector(vector<float> &_v)
    {
        int i = 0;
        if(paramVideoBufferInput.get()!=nullptr) recalculate(i);
    }
    
    //------------------------------------------------------------------
    void Multix3DFilter::changedManualOffsetMs(float &f)
    {
        int i=0;
        recalculate(i);
    }
    
    //------------------------------------------------------------------

    void Multix3DFilter::setNumHeaders(int _numHeaders){
        paramNumHeaders=_numHeaders;
    }

    //------------------------------------------------------------------
    int Multix3DFilter::getNumHeaders(){
        return paramNumHeaders;
    }

    
    //-----------------------------------------
    void ofxPm::Multix3DFilter::recalculate(int &_i)
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
            if(oneCopyMs!=paramManualOffsetMs) paramManualOffsetMs = oneCopyMs;
        }
        
        else if(!paramUseBPM)
        {
            oneCopyMs = paramManualOffsetMs;
        }
        //this->setNumHeaders(paramNumHeaders);
    
        for(int i=0;i<paramNumHeaders;i++)
        {
            if(paramLinearDistribution)
            {
                // in linear distribution, the copies are spaced equally a time/distance defined by BPM and beatMult/Div
                vf.push_back(i*oneCopyMs);
            }
            else
            {
                // non-linear distribution, the copies are distributed from 0 to the time expressed by BPM/Mult/Div.
                // And inside this period the copies are distributed by Generator
                // TO DO
//                float outMin,outMax;
                
//                int oscillatorReIndex = int(ofMap(i,0,float(paramNumHeaders),0,float(paramVideoBufferInput.get()->getSizeInFrames())));
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
            }
        }
            this->updateValuesMs(vf);

        
    }
    

    //--------------------------------------------------------
    void Multix3DFilter::updateValuesMs(vector<float> _vf)
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
            if(multixDelaysInMs.size()>0) multixDelaysInMs[0] = 0.0;


        }

    }
    
//--------------------------------------------------------
void Multix3DFilter::updateValuesPct(vector<float> _vf)
{

}
    
//--------------------------------------------------------
VideoFrame Multix3DFilter::getNextVideoFrame()
{

    return frame;
}

//--------------------------------------------------------
void Multix3DFilter::newVideoFrame(VideoFrame & _frame)
{
    //set buffer size ?
    int desiredBufferSize = 900;
    if(paramVideoBufferInput.get()->getBufferSize()!=desiredBufferSize)
    {
        paramVideoBufferInput.get()->setBufferSize(desiredBufferSize);
    }

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
void Multix3DFilter::drawIntoFbo(int x, int y,int w, int h)
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
    float opacFromVec = 1.0;
    bool anyOversize=false;
    float movingOnX=0.0;
	for(int i = paramNumHeaders-1; i>=0; i--)
    {
        // if delay time of each videoRenderer is in the right range of Ms (0..TotalMs)
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

        // if it's inside the buffer range (>0 and <buffSize)
        if((multixDelaysInMs[i]>=0)&&(multixDelaysInMs[i] < totalBufferSizeInMs))
        {
            headersInAction++;
            videoHeader.setDelayMs(multixDelaysInMs[i]);
            VideoFrame vf = videoHeader.getNextVideoFrame();
            
            ofVec2f frameResolution = ofVec2f(vf.getWidth(),vf.getHeight());
            ofVec2f screenResoltion = ofVec2f(1920,1080);
            
            // TRANSFORM VEC
            /////////////////
            if(i < paramCopiesPositionX.get().size())
            {
                movingOnX = paramCopiesPositionX.get().at(i);
            }
            else
            {
                movingOnX = 0;
            }
            // OPACITY VEC
            ////////////////
            if(i < paramCopiesOpacity.get().size())
            {
                opacFromVec = paramCopiesOpacity.get().at(i);
            }
            else
            {
                opacFromVec = 1.0;
            }

            
            ofPushMatrix();
            
            //ofTranslate(0,-(paramScale*screenResoltion.y/2.0) + (frameResolution.y/2.0),0);
            ofTranslate(movingOnX,0);
            ofTranslate(screenResoltion.x/2.0,screenResoltion.y/2.0,0);
            // rotate to portrait
            ofRotate(90,0,0, 1);
            // flip x
            ofRotate(180,1,0,0);
            ofTranslate((-frameResolution.x*paramScale)/2.0,(-frameResolution.y*paramScale)/2.0,0);

            
            if(!vf.isNull())
            {
                ofSetColor((opac*opacFromVec*255.0),255.0);
                vf.getTextureRef().draw(x,y,frameResolution.x * paramScale,frameResolution.y * paramScale);
            }
            paramOversize =0;
            ofPopMatrix();
        }
        else
        {
            //cout << "Multix3DFilter:: Out of time in Ms range !! Copy : " << i << "Delay in Ms = " << multixDelaysInMs[i] << " Max Buffer Size : " << paramVideoBufferInput.get()->getMaxSize() << endl;
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


bool Multix3DFilter::isMinmaxBlend() const
{
    return paramMinMaxBlend;
}
 
    //-----------------------------------------
    void Multix3DFilter::changedVideoBuffer(ofxPm::VideoBufferNodeBased* &_videoBuffer)
    {
        if(_videoBuffer!=NULL)
        {
            // setup Headers
            videoHeader.setup(paramVideoBufferInput.get());
            videoHeader.setDelayMs(0.0);

            ofxPm::VideoFrame vf;
            newVideoFrame(vf);
        }
    }

    
}

