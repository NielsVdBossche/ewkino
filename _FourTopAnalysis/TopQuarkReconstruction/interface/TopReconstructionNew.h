#ifndef TOPRECONEW_H
#define TOPRECONEW_H

#include "../../FourTopEventHandling/interface/EventFourT.h"
#include "../../../constants/particleMasses.h"

class TopReconstructionNew {
    private:
        EventFourT* selection;

        // each pair is a mTop, mW pair. Might change this to abs(m-mTop), abs(m-mW)
        std::pair<double, double> bestTop, secondBestTop;
    public:
        TopReconstructionNew(EventFourT* selection) : selection(selection) {};
        ~TopReconstructionNew();

        void RecoBestTwoTops();
        std::pair<double, double> getBestTop(std::vector<unsigned>& excludedJets);

        std::pair<double, double> getBestRecoTop() {return bestTop;}
        std::pair<double, double> getSecondBestRecoTop() {return secondBestTop;}
};


#endif