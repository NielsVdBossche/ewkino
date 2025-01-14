#ifndef HISTINFO_2D_H
#define HISTINFO_2D_H

#include <TH2.h>

#include "HistInfo.h"

class HistInfo_2D : public HistInfo {
    private:
        std::string yLabel;
        unsigned nYBins;
        double yMin, yMax;
        double maxYBinC;
        double yBinWidth;

        void setMaxYBinCenter() {maxYBinC = yMax - 0.5*(yMax - yMin)/nYBins;}
        void setYBinWidth() {yBinWidth = (yMax - yMin)/nYBins;}

    public:
        HistInfo_2D() {HistInfo();};

        HistInfo_2D(const HistInfo_2D& h1) : HistInfo(h1) {
            yLabel = h1.yLabel;
            nYBins = h1.nYBins;
            yMin = h1.yMin;
            yMax = h1.yMax;
            maxYBinC = h1.maxYBinC;
            yBinWidth = h1.yBinWidth;
        }

        HistInfo_2D(const std::string& name, const std::string& x, unsigned xbins, double xmin, double xmax, const std::string& y, unsigned ybins, double ymin, double ymax) :
            HistInfo(name, x, xbins, xmin, xmax), yLabel(y), nYBins(ybins), yMin(ymin), yMax(ymax)
        {
            setMaxYBinCenter();
            setYBinWidth();
        }

        virtual TH2D* makeHistPtr_2D(const std::string& histName) const {
            TH2D* hist = new TH2D( histName.c_str(), ( histName + ";" + xLabel + ";" + yLabel + ";Events").c_str(),  nBins, xMin, xMax, nYBins, yMin, yMax);
            hist->Sumw2();
            
            return hist;
        }

        virtual std::shared_ptr<TH2D> makeHist_2D(const std::string& histName) const {
            std::shared_ptr<TH2D> hist = std::make_shared<TH2D>( histName.c_str(), ( histName + ";" + xLabel + ";" + yLabel + ";Events").c_str(),  nBins, xMin, xMax, nYBins, yMin, yMax);
            hist->Sumw2();
            
            return hist;
        }

        virtual bool is2D() {return true;}

};

#endif
