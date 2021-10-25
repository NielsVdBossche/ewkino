#include "../interface/Uncertainty.h"

UncertaintyWrapper::UncertaintyWrapper(EventSelection4T* selection, TreeReader* reader) : selection(selection), treeReader(reader) {

}

std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* UncertaintyWrapper::uncertaintyHistogramInit(std::vector<HistInfo>* info, bool up) {
    std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>* output = new std::map<shapeUncertaintyIdentifier, std::vector< std::vector<std::shared_ptr<TH1D>>>>();

    unsigned uncertainty = 0;
    if (up) uncertainty++;

    std::vector< Sample > sampleVec = treeReader->sampleVector();

    for (; uncertainty < shapeUncertaintyIdentifier::end; uncertainty += 2) {
        (*output)[(shapeUncertaintyIdentifier)up] = *histHelper::initHistograms(info, sampleVec);
    }

    return output;
}

void UncertaintyWrapper::initDL(std::vector<HistInfo>* dlInfo) {
    histogramsUnc_info_DL = dlInfo;
    // Functie taken in infovector and building uncertainty histograms

    histogramsUncDown_DL = uncertaintyHistogramInit(dlInfo, false);
    histogramsUncUp_DL = uncertaintyHistogramInit(dlInfo, true);
}

void UncertaintyWrapper::initML(std::vector<HistInfo>* mlInfo) {
    histogramsUnc_info_ML = mlInfo;
    // Functie taken in infovector and building uncertainty histograms

    histogramsUncDown_ML = uncertaintyHistogramInit(mlInfo, false);
    histogramsUncUp_ML = uncertaintyHistogramInit(mlInfo, true);
}

void UncertaintyWrapper::initCRZ(std::vector<HistInfo>* crzInfo) {
    histogramsUnc_info_CRZ = crzInfo;
    // Functie taken in infovector and building uncertainty histograms

    histogramsUncDown_CRZ = uncertaintyHistogramInit(crzInfo, false);
    histogramsUncUp_CRZ = uncertaintyHistogramInit(crzInfo, true);
}

void UncertaintyWrapper::initCRW(std::vector<HistInfo>* crwInfo) {
    histogramsUnc_info_CRW = crwInfo;
    // Functie taken in infovector and building uncertainty histograms

    histogramsUncDown_CRW = uncertaintyHistogramInit(crwInfo, false);
    histogramsUncUp_CRW = uncertaintyHistogramInit(crwInfo, true);
}

void UncertaintyWrapper::initCRO(std::vector<HistInfo>* croInfo) {
    histogramsUnc_info_CRO = croInfo;
    // Functie taken in infovector and building uncertainty histograms

    histogramsUncDown_CRO = uncertaintyHistogramInit(croInfo, false);
    histogramsUncUp_CRO = uncertaintyHistogramInit(croInfo, true);
}

