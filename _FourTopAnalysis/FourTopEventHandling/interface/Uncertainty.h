#ifndef UNCERTAINTY_4T_H
#define UNCERTAINTY_4T_H

#include "../../additionalTools/interface/HistogramManager.h"
#include "../../histogramSetups/histogramSetup.h"
#include "EventFourT.h"

// Shape uncertainties
// JEC and JER are year dependant
enum shapeUncertaintyIdentifier {
    jecDown,
    jecUp,
    jerDown,
    jerUp,
    end  /// always at the end for easier loops
};

// potential things to add: btag, met, leptons, scale, pileup, prefire, 


class Uncertainty : public HistogramManager {
private:
    shapeUncertaintyIdentifier id;

    Uncertainty* next = nullptr;

public:
    Uncertainty(std::string& channel, std::vector<HistInfo>* histInfo, int uncNumber);
    ~Uncertainty();

    Uncertainty* getNext() const {return next;}
    void setNext(Uncertainty* newNext) {next = newNext;}

    void applyUncertainty(EventFourT* event, bool nonPrompt, bool trilep, bool fourlep);

};

#endif
