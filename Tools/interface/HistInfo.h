#ifndef HistInfo_H
#define HistInfo_H

//include c++ library classes
#include <string>
#include <memory>
#include <iomanip>

//include ROOT classes
#include "TH1D.h"

//include other parts of code 
#include "stringTools.h"

class HistInfo{

    public:
        HistInfo() = default;
        HistInfo(const HistInfo& h1) {
            fileName = h1.fileName;
            xLabel = h1.xLabel;
            nBins = h1.nBins;
            xMin = h1.xMin;
            xMax = h1.xMax;
            binLabels = h1.binLabels;
            maxBinC = h1.maxBinC;
            binWidth = h1.binWidth;
        }

        //constructor takes an optional vector of strings to label the bins 
        HistInfo( const std::string& name, const std::string& x, unsigned bins, double min, double max, const std::vector< std::string>& binLabelList = std::vector<std::string>() ):
            fileName(name), xLabel(x), nBins(bins), xMin(min), xMax(max), binLabels(binLabelList)
        { 
            setMaxBinCenter();
            setBinWidth();
        }

        TH1D* makeHistPtr( const std::string& histName ) const{
            
            //make ylabel
            std::string yLabel = "Events";
            
            //don't add a binWidth to the y label if there is only one bin, or the x labels are custom text
            if( nBins > 1 && binLabels.empty() ){
                yLabel += (" / " + stringTools::doubleToString( getBinWidth(), 2 ) );
                //check if the xLabel has a unit, if so add it to the y label too
                if( xLabel.find("GeV") != std::string::npos ){
                    yLabel += " GeV";                    
                }
            }

    
            TH1D* hist = new TH1D( histName.c_str(), ( histName + ";" + xLabel + ";" + yLabel).c_str(),  nBins, xMin, xMax);
            hist->Sumw2();

            //set bin labels if they were initialized 
            if( !( binLabels.empty() ) ){
            	for(unsigned bin = 0; bin < nBins; ++bin){
        			hist->GetXaxis()->SetBinLabel( bin + 1, binLabels[bin].c_str() );
    			}
            }
            return hist;
        }

        std::shared_ptr<TH1D> makeHist( const std::string& histName ) const{
            
            //make ylabel
            std::string yLabel = "Events";
            
            //don't add a binWidth to the y label if there is only one bin, or the x labels are custom text
            if( nBins > 1 && binLabels.empty() ){
                yLabel += (" / " + stringTools::doubleToString( getBinWidth(), 2 ) );
                //check if the xLabel has a unit, if so add it to the y label too
                if( xLabel.find("GeV") != std::string::npos ){
                    yLabel += " GeV";                    
                }
            }

    
            std::shared_ptr<TH1D> hist = std::make_shared<TH1D>( histName.c_str(), ( histName + ";" + xLabel + ";" + yLabel).c_str(),  nBins, xMin, xMax);
            hist->Sumw2();

            //set bin labels if they were initialized 
            if( !( binLabels.empty() ) ){
            	for(unsigned bin = 0; bin < nBins; ++bin){
        			hist->GetXaxis()->SetBinLabel( bin + 1, binLabels[bin].c_str() );
    			}
            }
            return hist;
        }

        virtual bool is2D() {return false;}

        std::string name() const { return fileName; }
        double maxBinCenter() const { return maxBinC; }

        void setName(std::string newName) {fileName = newName;}

    protected:
        std::string fileName;
        std::string xLabel;
        unsigned nBins;
        double xMin, xMax;
        std::vector< std::string > binLabels;
        double maxBinC;
        double binWidth;

        void setMaxBinCenter() { maxBinC = xMax - 0.5*(xMax - xMin)/nBins; }
        void setBinWidth() { binWidth = (xMax - xMin)/nBins; }
        double getBinWidth() const{ return binWidth; }
};
#endif
