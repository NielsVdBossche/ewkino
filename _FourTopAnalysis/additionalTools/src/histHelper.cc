#include "../interface/histHelper.h"

std::vector< std::vector< std::shared_ptr<TH1D>>>* histHelper::initHistograms(std::vector<HistInfo>* histInfoVec, std::vector<Sample>& sampleVec) {
    std::vector< std::vector< std::shared_ptr<TH1D>>>* ret = new std::vector< std::vector< std::shared_ptr<TH1D>>>(histInfoVec->size(), std::vector< std::shared_ptr< TH1D > >( sampleVec.size() + 1 ) );

    for( size_t dist = 0; dist < histInfoVec->size(); ++dist ){
        for( size_t p = 0; p < sampleVec.size() + 1; ++p ){
            if( p < sampleVec.size() ){
                (*ret)[ dist ][ p ] = histInfoVec->at(dist).makeHist( histInfoVec->at(dist).name() + "_" + sampleVec[p].uniqueName() );
            } else {
                //(*ret)[ dist ][ p ] = histInfoVec->at(dist).makeHist( histInfoVec->at(dist).name() + "_nonprompt" );
            }
        }
    }
}
