#include "../interface/CombinedSampleReweighter.h"

void CombinedSampleReweighter::addReweighter( const std::string& name, const std::shared_ptr< ReweighterSample >& reweighter ){
    // check if exists already if necessary
    
    reweighterMap[ name ] = reweighter;
    reweighterVector.push_back( reweighter );
}

double CombinedSampleReweighter::totalWeight( double (ReweighterSample::*weightFunction)(const Event&, const int) const, const Event& event, const int njet) const {
    double weight = 1.;
    for( const auto& r : reweighterVector){
        weight *= ((*r).*weightFunction)( event, njet );
    }
    return weight;
}

double CombinedSampleReweighter::totalWeight( const Event& event, const int njet ) const{
    return totalWeight(&ReweighterSample::weight, event, njet);
}

double CombinedSampleReweighter::totalWeightUp( const Event& event, const int njet ) const{
    return totalWeight(&ReweighterSample::weightUp, event, njet);
}
double CombinedSampleReweighter::totalWeightDown( const Event& event, const int njet ) const{
    return totalWeight(&ReweighterSample::weightDown, event, njet);
}

bool CombinedSampleReweighter::IsEmpty() const {
    if (reweighterVector.size() == 0) {
        return true;
    }
    return false;
}
