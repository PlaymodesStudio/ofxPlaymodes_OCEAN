
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
        parameters->add(paramLocalTranslate.set("Translate Locally", false));
        parameters->add(paramCopiesPositionX.set("Copies Position X",{0.5},{0},{1}));
        parameters->add(paramCopiesPositionY.set("Copies Position Y",{0.5},{0},{1}));
        parameters->add(paramCopiesPositionZ.set("Copies Position Z",{0.5},{0},{1}));
        parameters->add(paramCopiesScale.set("Copies Scale",{1},{0},{10}));
        parameters->add(paramCopiesRotationX.set("Copies Rotation X",{0},{0},{1}));
        parameters->add(paramCopiesRotationY.set("Copies Rotation Y",{0},{0},{1}));
        parameters->add(paramCopiesRotationZ.set("Copies Rotation Z",{0},{0},{1}));

        parameters->add(paramCopiesOpacity.set("Copies Opacity",{1.0},{0},{1}));
        parameters->add(paramLinearDistribution.set("Linear Distribution",true));
        parameters->add(paramDistributionVector.set("Distribution Vector",{0},{0},{1}));
        parameters->add(paramOversize.set("Oversize",0,0,1));
        
        parameters->add(paramFrameOut.set("Frame Output", frame));

        listeners.push(paramOffsetBeatDiv.newListener(this, &Multix3DFilter::recalculate));
        listeners.push(paramOffsetBeatMult.newListener(this, &Multix3DFilter::recalculate));
        listeners.push(paramNumHeaders.newListener(this,&Multix3DFilter::recalculate));
        listeners.push(paramManualOffsetMs.newListener(this,&Multix3DFilter::changedManualOffsetMs));
        listeners.push(paramVideoBufferInput.newListener(this, &Multix3DFilter::changedVideoBuffer));
        listeners.push(paramDistributionVector.newListener(this,&Multix3DFilter::changedDistributionVector));
//        listeners.push(paramCopiesPositionX.newListener(this,&::ofxPm::Multix3DFilter::changedCopiesPositionX));

        int i=0;
        recalculate(i);
        

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

    if(paramNumHeaders>0)
    {
        fbo.begin();
        {
            if(paramMinMaxBlend) ofClear(0,0,0,255);
            else ofClear(255,255,255,255);
            
            ofSetColor(255);
            drawIntoFbo(0,0,fbo.getWidth(),fbo.getHeight());
            //ofDrawCircle(0,0,200);
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
    float movingOnY=0.0;
    float movingOnZ=0.0;
    float rotatingOnX=0.0;
    float rotatingOnY=0.0;
    float rotatingOnZ=0.0;
    float scaling=0.0;

    ofVec2f frameResolution;
    ofVec2f screenResolution = ofVec2f(1920,1200);

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
            if(!vf.isNull()){
                frameResolution = ofVec2f(vf.getWidth(),vf.getHeight());
                
                // TRANSFORM VEC
                /////////////////
                // T X
                //////
                movingOnX = getValueForPosition(paramCopiesPositionX.get(), i);
                // T Y
                //////
                movingOnY = getValueForPosition(paramCopiesPositionY.get(), i);
                // T Z
                //////
                movingOnZ = getValueForPosition(paramCopiesPositionZ.get(), i);
                // ROTATE VEC
                /////////////////
                // R X
                //////
                rotatingOnX = getValueForPosition(paramCopiesRotationX.get(), i);
                // R Y
                //////
                rotatingOnY = getValueForPosition(paramCopiesRotationY.get(), i);
                // R Z
                //////
                rotatingOnZ = getValueForPosition(paramCopiesRotationZ.get(), i);
                // SCALE
                //////////
                scaling = getValueForPosition(paramCopiesScale.get(), i);
                // OPACITY VEC
                ////////////////
                opacFromVec = getValueForPosition(paramCopiesOpacity.get(), i);
                
                ofPushMatrix();
                
                ofTranslate(w/2.0,h/2.0,0);
                
                if(!paramLocalTranslate){
                    ofTranslate(ofMap(movingOnX, 0, 1, -w/2, w/2), ofMap(movingOnY, 0, 1, -h/2, h/2), ofMap(movingOnZ, 0, 1, -1000, 1000));
                }
                
                ofRotateDeg(rotatingOnX*360,1,0, 0);
                ofRotateDeg(rotatingOnY*360,0,1, 0);
                ofRotateDeg(rotatingOnZ*360,0,0, 1);
                
                if(paramLocalTranslate){
                    ofTranslate(ofMap(movingOnX, 0, 1, -w/2, w/2), ofMap(movingOnY, 0, 1, -h/2, h/2), ofMap(movingOnZ, 0, 1, -1000, 1000));
                }
                
                // scale
                ofScale(scaling);
                // rotate to user defined
                
                
                // rotate to portrait
                //TODO: VIDEOFRAME ROTATION
                //ofRotate(90,0,0, 1);
                // flip x
                //ofRotate(180,1,0,0);
                //ofScale(scaling);
                //ofTranslate((-frameResolution.x*paramScale)/2.0,(-frameResolution.y*paramScale)/2.0,0);

                //ofPushStyle();
                ofSetRectMode(OF_RECTMODE_CENTER);
                ofSetColor((opac*opacFromVec*255.0),255.0);
                vf.getTextureRef().draw(x,y,frameResolution.x,frameResolution.y);
//                cout << "Multix3D : i : " << i << " DRAWING at delay Ms  " << multixDelaysInMs[i] << " at Position " << movingOnX <<" ," << movingOnY << " , " << movingOnZ << " Scale : " << scaling << endl;
                //ofPopStyle();
            }
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
 
//    //-----------------------------------------
//    void Multix3DFilter::changedVideoBuffer(ofxPm::VideoBufferNodeBased* &_videoBuffer)
//    {
//        if(_videoBuffer!=NULL)
//        {
//            // setup Headers
//            videoHeader.setup(paramVideoBufferInput.get());
//            videoHeader.setDelayMs(0.0);
//
//            ofxPm::VideoFrame vf;
//            newVideoFrame(vf);
//        }
//    }
    
    void Multix3DFilter::changedVideoBuffer(ofxPm::VideoBufferNodeBased* &_videoBuffer)
    {
        if(_videoBuffer!=NULL)
        {
            // allocate fbo where to draw
            if (fbo.getWidth()<=0)
            {
                // setup FBO
                int resX = paramVideoBufferInput.get()->getWidth();
                int resY = paramVideoBufferInput.get()->getHeight();
                //TODO: publish fbo dimensions?
                fbo.allocate(1920,1200,GL_RGB);
                
                
                // setup Headers
                videoHeader.setup(paramVideoBufferInput.get());
                videoHeader.setDelayMs(0.0);
                
            }
            
            ofxPm::VideoFrame vf;
            newVideoFrame(vf);
        }
    }


    
}

