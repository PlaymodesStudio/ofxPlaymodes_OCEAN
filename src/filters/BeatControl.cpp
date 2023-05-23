
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

        addParameter(paramDivOut.set("Div Out",1,1,32));
        addParameter(paramMultOut.set("Mult Out",1,1,32));

        addParameterDropdown(paramBeatsDropDown, "Beats", 5, {"1/32","1/16","1/8", "1/4", "1/2", "1","2","4","8","16","32"});

        listener = paramBeatsDropDown.newListener(this, &BeatControl::changedBeats);
        
    }
    //------------------------------------------------------------
    void BeatControl::changedBeats(int &i)
    {
        switch(i)
        {
            case 0:
                paramDivOut=32;
                paramMultOut=1;
                break;
            case 1:
                paramDivOut=16;
                paramMultOut=1;
                break;
            case 2:
                paramDivOut=8;
                paramMultOut=1;
                break;
            case 3:
                paramDivOut=4;
                paramMultOut=1;
                break;
            case 4:
                paramDivOut=2;
                paramMultOut=1;
                break;
            case 5:
                paramDivOut=1;
                paramMultOut=1;
                break;
            case 6:
                paramDivOut=1;
                paramMultOut=2;
                break;
            case 7:
                paramDivOut=1;
                paramMultOut=4;
                break;
            case 8:
                paramDivOut=1;
                paramMultOut=8;
                break;
            case 9:
                paramDivOut=1;
                paramMultOut=16;
                break;
            case 10:
                paramDivOut=1;
                paramMultOut=32;
                break;
        }
    }



}
