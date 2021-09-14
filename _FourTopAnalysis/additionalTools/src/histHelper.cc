#include "../interface/histHelper.h"

std::vector< std::vector< std::shared_ptr<TH1D>>>* histHelper::initHistograms(std::vector<HistInfo>* histInfoVec, std::vector<Sample>& sampleVec) {
    std::vector< std::vector< std::shared_ptr<TH1D>>>* ret = new std::vector< std::vector< std::shared_ptr<TH1D>>>(sampleVec.size() + 1, std::vector< std::shared_ptr< TH1D > >(histInfoVec->size()));

    /*
    for( size_t dist = 0; dist < histInfoVec->size(); ++dist ){
        for( size_t p = 0; p < sampleVec.size() + 1; ++p ){
            if( p < sampleVec.size() ){
                (*ret)[ dist ][ p ] = histInfoVec->at(dist).makeHist( histInfoVec->at(dist).name() + "_" + sampleVec[p].uniqueName() );
            } else {
                //(*ret)[ dist ][ p ] = histInfoVec->at(dist).makeHist( histInfoVec->at(dist).name() + "_nonprompt" );
            }
        }
    }
    */

    for( size_t p = 0; p < sampleVec.size() + 1; ++p ){
        for( size_t dist = 0; dist < histInfoVec->size(); ++dist ){
            if( p < sampleVec.size() ){
                ret->at(p)[dist] = histInfoVec->at(dist).makeHist( histInfoVec->at(dist).name() + "_" + sampleVec[p].uniqueName() );
            } else {
                //(*ret)[ dist ][ p ] = histInfoVec->at(dist).makeHist( histInfoVec->at(dist).name() + "_nonprompt" );
            }
        }
    }

    return ret;

}

void histHelper::histFiller(std::vector<double>& fillVec, std::vector<std::shared_ptr<TH1D>>* hists, double weight) {
    for (size_t dist = 0; dist < fillVec.size(); dist++) {
        histogram::fillValue(hists->at(dist).get(), fillVec[dist], weight);
    }
}
