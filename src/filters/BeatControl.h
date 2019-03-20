
#ifndef BEATCONTROL_H
#define BEATCONTROL_H

#include "VideoFilter.h"
#include "ofxOceanodeNodeModel.h"

namespace ofxPm{
    

    class BeatControl: public ofxOceanodeNodeModel
    {
    public:
        
        BeatControl();
        virtual ~BeatControl();
        void setupNodeBased();
        
    private:
        
        ofParameter<int>                    paramBeatsDropDown;
        ofParameter<int>                    paramDivOut;
        ofParameter<int>                    paramMultOut;
        vector<vector<int>>                 beats;

        void changedBeats(int &i);
        
        ofEventListener listener;
};
}

#endif
