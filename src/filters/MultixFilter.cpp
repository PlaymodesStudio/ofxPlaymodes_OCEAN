
#include "MultixFilter.h"
#include "ofxOceanodeContainer.h"

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
        parameters->add(paramVideoBufferInput.set("Buffer Input", nullptr));
        parameters->add(paramNumHeaders.set("Num Headers",1,1,480));
        parameters->add(paramOpacityMode.set("Opacity Mode",0,0,2));
        parameters->add(paramMinMaxBlend.set("MinMax Blend",true));
        parameters->add(paramUseBPM.set("Use BPM",true));
        parameters->add(paramOffsetBeatDiv.set("Beats Div",1,1,32));
        parameters->add(paramOffsetBeatMult.set("Beats Mult",1,1,32));
        parameters->add(paramManualOffsetMs.set("Manual Offset Ms",33.0,0.0,4000.0));
        parameters->add(paramLinearDistribution.set("Linear Distribution",true));
        parameters->add(paramDistributionVector.set("Distribution Vector",{0},{0},{1}));
        parameters->add(paramFrameOut.set("Frame Output", frame));

        paramOffsetBeatDiv.addListener(this, &MultixFilter::recalculate);
        paramOffsetBeatMult.addListener(this, &MultixFilter::recalculate);
        paramNumHeaders.addListener(this,&MultixFilter::recalculate);
        paramVideoBufferInput.addListener(this, &MultixFilter::changedVideoBuffer);
        paramDistributionVector.addListener(this,&MultixFilter::changedDistributionVector);
        
        int i=0;
        recalculate(i);

    }
    //------------------------------------------------------------
    void MultixFilter::update(ofEventArgs &e)
    {
        if(fboHasToBeAllocated != glm::vec2(-1, -1))
        {
            fbo.allocate(fboHasToBeAllocated.x, fboHasToBeAllocated.y);
            fboHasToBeAllocated = glm::vec2(-1, -1);
        }
    }

    //--------------------------------------------------------------
    ofxOceanodeAbstractConnection* MultixFilter::createConnectionFromCustomType(ofxOceanodeContainer& c, ofAbstractParameter& source, ofAbstractParameter& sink)
    {
        if(source.type() == typeid(ofParameter<ofxPm::VideoBufferNodeBased*>).name())
        {
            return c.connectConnection(source.cast<ofxPm::VideoBufferNodeBased*>(), sink.cast<ofxPm::VideoBufferNodeBased*>());
        }
        return nullptr;
    }

    //------------------------------------------------------------------
    void MultixFilter::changedDistributionVector(vector<float> &_v)
    {
        int i = 0;
        if(paramVideoBufferInput.get()!=nullptr) recalculate(i);
    }
    //------------------------------------------------------------------

    void MultixFilter::setNumHeaders(int _numHeaders){
        paramNumHeaders=_numHeaders;
    }

    //------------------------------------------------------------------
    int MultixFilter::getNumHeaders(){
        return paramNumHeaders;
    }

    
    //-----------------------------------------
    void ofxPm::MultixFilter::recalculate(int &_i)
    {
        
//        float gBPM = parametersControl::getInstance().getGlobalBPM();
        float gBPM = 120.0;
        float BPMfactor;
        if(paramOffsetBeatDiv!=0)
        {
            BPMfactor = (float(paramOffsetBeatMult)/float(paramOffsetBeatDiv));
        }
        else  BPMfactor = 1.0;

        float oneBeatMs = (60.0/gBPM)*1000;
        float oneCopyMs = oneBeatMs / BPMfactor;
    
        vector<float> vf;
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
                
                
                //            int oscillatorReIndex = int(ofMap(i,0,float(guiNumCopies),0,float(numOscillatorBanks)));
                //            mutex.lock();
                //            {
                //                vf.push_back(ofMap(guiMultixValues.get()[oscillatorReIndex],0.0,1.0, 0.0, oneCopyMs));
                //            }
                //            mutex.unlock();

                
                // non-linear distribution, the copies are distributed from 0 to the time expressed by BPM/Mult/Div.
                // And inside this period the copies are distributed by Generator
                // TO DO
//                float outMin,outMax;
                
                int oscillatorReIndex = int(ofMap(i,0,float(paramNumHeaders),0,float(paramVideoBufferInput.get()->getSizeInFrames())));
//                int oscillatorReIndex = int(ofMap(i,0,float(paramNumHeaders),0,float(numOscillatorBanks)));
                float m;
                mutex.lock();
                {
                    m = paramDistributionVector.get().at(oscillatorReIndex);
                    vf.push_back(ofMap(m,0.0,1.0, 0.0, oneCopyMs));
                    //vf.push_back(ofMap(guiMultixValues.get()[oscillatorReIndex],0.0,1.0, 0.0, oneCopyMs));
                }
                mutex.unlock();
               //cout << i << " ::  OscillatorReindex: " << oscillatorReIndex << " // m : " << m << " :: maxSize : " << paramVideoBufferInput.get()->getMaxSize()<< endl;
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
            if(multixDelaysInMs.size()>0) multixDelaysInMs[0] = 0.0;

//            // CLEAN headers and renderers and resize the to new
//            //setNumHeaders(_vf.size());
//            int currNumHeaders = videoHeader.size();
//            if(currNumHeaders!=paramNumHeaders){
//                videoHeader.resize(paramNumHeaders);
//                videoRenderer.resize(paramNumHeaders);
//                for(int i=0;i<paramNumHeaders;i++)
//                {
//                    videoHeader[i].setup(paramVideoBufferInput.get());
//                    //videoRenderer[i].setup(videoHeader[i]);
//                }
//                cout << "Multix::Renderer::Update Ms WARNING : currNumHeaders!=numHeaders" << endl;
//            }
//            
//            // update the delayMs for each
//            for(int i=_vf.size()-1; i>=0; i--)
//            {
//                videoHeader[i].setDelayMs(_vf[i]);
//            }
//            videoHeader[0].setDelayMs(0);

        }

    }
    
//--------------------------------------------------------
void MultixFilter::updateValuesPct(vector<float> _vf)
{
//    int currNumHeaders = videoHeader.size();
//    if(currNumHeaders!=numHeaders){
//        videoHeader.resize(numHeaders);
//        videoRenderer.resize(numHeaders);
//        for(int i=0;i<numHeaders;i++){
//            videoHeader[i].setup(*videoBuffer);
//            videoRenderer[i].setup(videoHeader[i]);
//        }
//        cout << "Multix::Renderer::Update Pct WARNING : currNumHeaders!=numHeaders" << endl;
//    }
//    
//    for(int i=videoRenderer.size()-1; i>=0; i--)
//    {
//        //videoHeader[i].setDelayMs(delayOffset*1000*i);
//        
//        float oneFrameMs = (1.0 / 60.0) * 1000.0;
//        float totalLengthInMs = videoBuffer->getMaxSize() * oneFrameMs;
//        
//        float myDelayMs = ofMap(_vf[i],0.0,1.0,0.0,delayOffset*100*videoBuffer->getMaxSize());
//        //float myDelayMs = delayOffset*100*i;
//        
//        
//        videoHeader[i].setDelayMs(myDelayMs);
//        
//        // cout << "Updating Values VF [" << i << "]" << " : " << _vf[i] << " VALUE : " << myDelayMs << " Total Length in Ms :" << totalLengthInMs << endl;
//        
//        
//        //cout << i << " : " << myDelayMs << ",";
//        //        videoRenderer[i].setTint(tint);
//        //        videoRenderer[i].setMinmaxBlend(minmaxBlend);
//    }
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
    //ofEnableAlphaBlending();

    if(paramMinMaxBlend)
    {
        glBlendEquationEXT(GL_MAX);
    }else{
        glBlendEquationEXT(GL_MIN);
    }

    int headersInAction = 0;
    float oneFrameMs	= 1000.0 / paramVideoBufferInput.get()->getFps();
    float totalBufferSizeInMs = paramVideoBufferInput.get()->getMaxSize() * oneFrameMs;

    float opac = 1.0;
	for(int i = paramNumHeaders; i>=0; i--)
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
        //cout << "MultixFilter :: i : " << i << " Opac[0..1] : " << opac << endl;

        if((multixDelaysInMs[i]>=0)&&(multixDelaysInMs[i] < totalBufferSizeInMs))
        {
            headersInAction++;
            ofSetColor((opac*255.0),255);
            // or the opposite order size-i-1 ? or just "i"
            //videoRenderer[i].draw(x,y,w,h);
            videoHeader.setDelayMs(multixDelaysInMs[i]);
            VideoFrame vf = videoHeader.getNextVideoFrame();
            cout << "MultixFilter:: Copy : " << i << " Delayed : " << multixDelaysInMs[i] << " And Buffer is " << paramVideoBufferInput.get()->getSizeInFrames() << endl;
            if(!vf.isNull()) vf.getTextureRef().draw(x,y,w,h);
        }
	}
    //cout << "MultixFilter :: Headers in Action : " << headersInAction << endl;
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


bool MultixFilter::isMinmaxBlend() const
{
    return paramMinMaxBlend;
}
 
    //-----------------------------------------
    void MultixFilter::changedVideoBuffer(ofxPm::VideoBufferNodeBased* &_videoBuffer)
    {
        // allocate fbo where to draw
        if ((fbo.getWidth()<=0) && (paramVideoBufferInput.get()->getWidth()) )
        {
            // setup FBO
            int resX = paramVideoBufferInput.get()->getWidth();
            int resY = paramVideoBufferInput.get()->getHeight();
            fbo.allocate(resX,resY,GL_RGBA);
            
            
            // setup Headers
            videoHeader.setup(paramVideoBufferInput.get());
            videoHeader.setDelayMs(0.0);
        }
    
        ofxPm::VideoFrame vf;
        newVideoFrame(vf);

    }

    
}

