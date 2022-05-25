#include "../interface/EventFourT.h"

// originally taken from https://github.com/wverbeke/ewkino/blob/43a93413e42bb80406028da905017308264f4224/tZqAnalysisCode/eventselection/src/eventSelections.cc#L115-L173

bool EventFourT::hasLeptonFromMEExternalConversion(){
    for( const auto& leptonPtr : **mediumLeps){
        if( leptonPtr->isFO() && leptonFromMEExternalConversion( leptonPtr.get() ) ){
            return true;
        }
    }
    return false;
}

// help functions for overlap removal between inclusive and dedicated photon samples //

bool EventFourT::leptonFromMEExternalConversion( Lepton* lepton ){
    // from Willem's ewkinoAnalysis code
    if( !( lepton->matchPdgId() == 22 ) ) return false;
    if( !( lepton->isPrompt() && lepton->provenanceConversion() == 0 ) ) return false;
    return true;
}

bool EventFourT::passPhotonOverlapRemoval(){
    bool isPhotonSample = false;
    bool isInclusiveSample = false;
    std::string sampleName = event->sample().fileName();
    if( stringTools::stringContains( sampleName, "DYJetsToLL" ) 
	|| stringTools::stringContains( sampleName, "TTTo" ) 
	|| stringTools::stringContains( sampleName, "TTJets" ) ){
        isInclusiveSample = true;
    } else if( stringTools::stringContains( sampleName, "TTGamma" ) 
	|| stringTools::stringContains( sampleName, "ZGToLLG" ) 
	|| stringTools::stringContains( sampleName, "ZGTo2LG" )
	|| stringTools::stringContains( sampleName, "WGToLNuG" ) ){
        isPhotonSample = true;
    }

    if( !( isPhotonSample || isInclusiveSample ) ){
        return true;
    }

    bool usePhotonSample = false;
    // method 1: check for prompt leptons matched to photons without provenanceConversion
    if( hasLeptonFromMEExternalConversion()) usePhotonSample = true;
    // method 2: simply check if all leptons are prompt (note: need to select FO leptons first!)
    //if( leptonsArePrompt()) {
    //    // if all leptons are prompt -> use ZG sample
    //    usePhotonSample = true;
    //    // BUT potential problem in ZG samples with missing phase space (gen level photon cut)
    //    // solved for 2016 by using LoosePtlPtg sample (? still checking)
    //    // for 2017/2018, these samples have other problems, instead use DY for low pt part
    //    //if(event.is2016()) usePhotonSample = true;
    //    //else{}
//
    //    // Reasoning behind this:
    //    // DY has no prompt contribution here, so only nonprompt contribution and this should come from data.
    //    // ZG's prompt contribution comes from events where the lepton is also found to be prompt -> lepton comes from photon which is linked to ME
    //    // For TTG/ttbar:
    //    // ttbar has 3 contributions: tt+photon, tt with NP, tt with charge mis ID-> chargeflip
    //    // first is MC, others are dd. 
    //    // if we use dd backgrounds, it should be fine to use this prompt differentiation
    //    // if we use MC backgrounds, this gets more complicated -> if all leptons are prompt we should still check chargeflips
    //    // if a chargeflip occurs we should use TTTo samples
    //    // if not (i.e. all leptons are prompt but no flip) -> ttg
    //    // if not all leptons prompt we use inclusive
    //    if (selType == selectionType::MCAll && leptonsAreNotChargeFlip()) {
    //        usePhotonSample = true;
    //    } else if (selType == selectionType::MCAll) {
    //        usePhotonSample = false;
    //    }
    //}
//
    if (isInclusiveSample) {
        return !usePhotonSample;
    } else if(isPhotonSample) {
        return usePhotonSample;
    }

    return true;
}
