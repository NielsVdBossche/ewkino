#include "../interface/Uncertainty.h"

UncertaintyWrapper::UncertaintyWrapper(EventSelection4T* selection, TreeReader* reader) : selection(selection), treereader(reader) {

}

std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* UncertaintyWrapper::uncertaintyHistogramInit(std::vector<HistInfo>* info) {
    
}


void UncertaintyWrapper::initDL(std::vector<HistInfo>* dlInfo) {
    histogramsUnc_info_DL = dlInfo;
    // Functie taken in infovector and building uncertainty histograms

}

void UncertaintyWrapper::initML(std::vector<HistInfo>* mlInfo) {

}

void UncertaintyWrapper::initCRZ(std::vector<HistInfo>* crzInfo) {

}

void UncertaintyWrapper::initCRW(std::vector<HistInfo>* crwInfo) {

}

void UncertaintyWrapper::initCRO(std::vector<HistInfo>* croInfo) {

}

