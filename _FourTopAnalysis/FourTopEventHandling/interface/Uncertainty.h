#ifndef UNCERTAINTY_H
#define UNCERTAINTY_H

#include "../../additionalTools/interface/HistogramManager.h"
#include "UncertaintyManager.h"

class Uncertainty : public HistogramManager {
private:
    shapeUncertaintyIdentifier id;

    Uncertainty* next;

public:
    Uncertainty(/* args */);
    ~Uncertainty();

    Uncertainty* getNext() const {return next;}
    void setNext(Uncertainty* newNext) {next = newNext;}

};

#endif
