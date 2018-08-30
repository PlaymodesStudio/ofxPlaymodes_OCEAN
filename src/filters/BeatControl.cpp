
#include "BeatControl.h"

namespace ofxPm{
    
    //------------------------------------------------------------
    BeatControl::BeatControl(): ofxOceanodeNodeModel("Beat Control")
    {
        setupNodeBased();
    }

    //------------------------------------------------------------
    BeatControl::~BeatControl()
    {
    }

    //------------------------------------------------------------
    void BeatControl::setupNodeBased()
    {
        color = ofColor::orange;

        parameters = new ofParameterGroup();
        parameters->add(paramDivOut.set("Div Out",1,1,32));
        parameters->add(paramMultOut.set("Mult Out",1,1,32));

        parameters->add(createDropdownAbstractParameter("Beats", {"1/8", "1/4", "1/2", "1","2","4","8","16","32"}, paramBeatsDropDown));

        paramBeatsDropDown.addListener(this, &BeatControl::changedBeats);
        
    }
    //------------------------------------------------------------
    void BeatControl::changedBeats(int &i)
    {
        switch(i)
        {
            case 0:
                paramDivOut=8;
                paramMultOut=1;
                break;
            case 1:
                paramDivOut=4;
                paramMultOut=1;
                break;
            case 2:
                paramDivOut=2;
                paramMultOut=1;
                break;
            case 3:
                paramDivOut=1;
                paramMultOut=1;
                break;
            case 4:
                paramDivOut=1;
                paramMultOut=2;
                break;
            case 5:
                paramDivOut=1;
                paramMultOut=4;
                break;
            case 6:
                paramDivOut=1;
                paramMultOut=8;
                break;
            case 7:
                paramDivOut=1;
                paramMultOut=16;
                break;
            case 8:
                paramDivOut=1;
                paramMultOut=32;
                break;
        }
    }



}
