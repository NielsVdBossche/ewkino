#include "../interface/Lepton.h"
#include "../interface/LeptonSelector.h"

// include c++ library classes
#include <utility>
#include <stdexcept>


// constructor from TreeReader
Lepton::Lepton( const TreeReader& treeReader, 
		const std::string& leptonType,
		const unsigned leptonIndex,
		LeptonSelector* leptonSelector ){
    is2016Object = treeReader.is2016();
    is2016PreVFPObject = treeReader.is2016PreVFP();
    is2016PostVFPObject = treeReader.is2016PostVFP();
    is2017Object = treeReader.is2017();
    is2018Object = treeReader.is2018();
    selector = leptonSelector;
    generatorInfo = treeReader.isMC() ? new LeptonGeneratorInfo(treeReader, leptonType, leptonIndex) : nullptr;
    if(leptonType=="electron"){
	vector = LorentzVector(
	    treeReader._Electron_pt[leptonIndex],
	    treeReader._Electron_eta[leptonIndex],
	    treeReader._Electron_phi[leptonIndex],
	    -1. );
	_charge = treeReader._Electron_charge[leptonIndex];
	_dxy = treeReader._Electron_dxy[leptonIndex];
	_dz = treeReader._Electron_dz[leptonIndex];
    } else if(leptonType=="muon"){
	vector = LorentzVector(
            treeReader._Muon_pt[leptonIndex],
            treeReader._Muon_eta[leptonIndex],
            treeReader._Muon_phi[leptonIndex],
            -1. );
	_charge = treeReader._Muon_charge[leptonIndex];
        _dxy = treeReader._Muon_dxy[leptonIndex];
        _dz = treeReader._Muon_dz[leptonIndex];
    } else if(leptonType=="tau"){
	vector = LorentzVector(
	    treeReader._Tau_pt[leptonIndex],
	    treeReader._Tau_eta[leptonIndex],
	    treeReader._Tau_phi[leptonIndex],
            -1. );
	_charge = treeReader._Tau_charge[leptonIndex];
        _dxy = treeReader._Tau_dxy[leptonIndex];
        _dz = treeReader._Tau_dz[leptonIndex];
    } else{
	std::string msg = "ERROR in Lepton::Lepton:";
	msg += " unrecognized lepton type " + leptonType;
	throw std::invalid_argument(msg);
    }
    _uncorrectedPt = pt();
    _uncorrectedE = energy();
}


Lepton::Lepton( const Lepton& rhs, LeptonSelector* leptonSelector ) : 
    PhysicsObject( rhs ),
    _charge( rhs._charge ),
    _dxy( rhs._dxy ),
    _dz( rhs._dz ),
    generatorInfo( new LeptonGeneratorInfo( *(rhs.generatorInfo) ) ),
    selector( leptonSelector ),
    isConeCorrected( rhs.isConeCorrected ),
    _uncorrectedPt( rhs._uncorrectedPt ),
    _uncorrectedE( rhs._uncorrectedE )
    {}


Lepton::Lepton( Lepton&& rhs, LeptonSelector* leptonSelector ) noexcept :
    PhysicsObject( std::move( rhs ) ),
    _charge( rhs._charge ),
    _dxy( rhs._dxy ),
    _dz( rhs._dz ),
    generatorInfo( rhs.generatorInfo ),
    selector( leptonSelector ),
    isConeCorrected( rhs.isConeCorrected ),
    _uncorrectedPt( rhs._uncorrectedPt ),
    _uncorrectedE( rhs._uncorrectedE )
{
    rhs.generatorInfo = nullptr;
}


Lepton::~Lepton(){
    delete selector;
    if( hasGeneratorInfo() ){
        delete generatorInfo;
    }
}


void Lepton::copyNonPointerAttributes( const Lepton& rhs ){
    _charge = rhs._charge;
    _dxy = rhs._dxy;
    _dz = rhs._dz;
    isConeCorrected = rhs.isConeCorrected;
    _uncorrectedPt = rhs._uncorrectedPt;
    _uncorrectedE = rhs._uncorrectedE;
}


Lepton& Lepton::operator=( const Lepton& rhs ){

    //copy the PhysicsObject part of the Lepton
    PhysicsObject::operator=(rhs); 

    //copy non pointer members
    copyNonPointerAttributes( rhs );

    //selector can keep pointing to the current lepton and does not need to be copied!
    
    //need to create new LeptonGeneratorInfo object to ensure self assignment works 
    if( hasGeneratorInfo() ){
        LeptonGeneratorInfo* oldInfo = generatorInfo;
        generatorInfo = new LeptonGeneratorInfo( *rhs.generatorInfo );
        delete oldInfo;
    }
    return *this;
}


Lepton& Lepton::operator=( Lepton&& rhs ) noexcept{

    //move the PhysicsObject part of the lepton
    PhysicsObject::operator=( std::move(rhs) );

    //in case of self assignment the move assignment should do no work
    if( this != &rhs ){

        //copy non pointer members
        copyNonPointerAttributes( rhs );

        //selector can keep pointing to the current lepton and does not need to be moved!

        //move pointer to generator information and make sure to free old memory
        delete generatorInfo; 
        generatorInfo = rhs.generatorInfo;
        rhs.generatorInfo = nullptr;
        
    }
    return *this;
}


bool Lepton::checkGeneratorInfo() const{
    if( !hasGeneratorInfo() ){
	std::string msg = "ERROR in Lepton::checkGeneratorInfo:";
        msg += " trying to access generator information for a lepton that has no generator info!";
	throw std::runtime_error(msg);
    }
    return true;
}


bool Lepton::hasGenMatch() const{
    if( checkGeneratorInfo() ){ return generatorInfo->hasGenMatch(); }
    else{ return false; }
}


bool Lepton::isPrompt() const{
    if( checkGeneratorInfo() ){ return generatorInfo->isPrompt(); }
    else { return false; }
}


int Lepton::momPdgId() const{
    if( checkGeneratorInfo() ){
        return generatorInfo->momPdgId(); 
    } else {
        return 0;
    }
}


int Lepton::matchPdgId() const{
    if( checkGeneratorInfo() ){
        return generatorInfo->matchPdgId();
    } else {
        return 0;
    }
}


int Lepton::matchCharge() const{
    if( checkGeneratorInfo() ){
        return generatorInfo->matchCharge();
    } else {
        return 0;
    }
}


unsigned Lepton::provenance() const{
    if( checkGeneratorInfo() ){
        return generatorInfo->provenance();
    } else {
        return 0;
    }
}


unsigned Lepton::provenanceCompressed() const{
    if( checkGeneratorInfo() ){
        return generatorInfo->provenanceCompressed();
    } else {
        return 0;
    }
}


unsigned Lepton::provenanceConversion() const{
    if( checkGeneratorInfo() ){
        return generatorInfo->provenanceConversion();
    } else {
        return 0;
    }
}

bool Lepton::isChargeFlip() const{
    // veto leptons from photon conversions (not counted as charge flips)
    if( matchPdgId()==22 ) return false;
    // check if measured charge differs from generator charge
    return (charge() != matchCharge());
}


void Lepton::applyConeCorrection(){

    //make sure cone correction can only be applied once 
    if( isConeCorrected ) return;
    isConeCorrected = true;

    //only apply cone correction only to leptons that are FO and not tight
    //loose leptons that do not pass FO are not corrected to make sure the object-level pT thresholds in isFO() are respected, and because they are usually used for purposes not requiring a cone correction
    //Tight leptons should never receive a cone correction!
    if( isFO() && !isTight() ){
        double correctionFactor = selector->coneCorrection();
        setLorentzVector( pt()*correctionFactor, eta(), phi(), energy()*correctionFactor );
    }
}


bool sameFlavor( const Lepton& lhs, const Lepton& rhs ){
    return ( 
        ( lhs.isMuon() && rhs.isMuon() ) ||
        ( lhs.isElectron() && rhs.isElectron() ) ||
        ( lhs.isTau() && rhs.isTau() )
    );
}

bool oppositeFlavor( const Lepton& lhs, const Lepton& rhs ){
    return !sameFlavor(lhs, rhs);
}

bool sameSign( const Lepton& lhs, const Lepton& rhs ){
    return ( lhs.charge() == rhs.charge() );
}

bool oppositeSign( const Lepton& lhs, const Lepton& rhs ){
    return !sameSign(lhs, rhs);
}

bool oppositeSignSameFlavor( const Lepton& lhs, const Lepton& rhs ){
    return ( sameFlavor( lhs, rhs ) && ( lhs.charge() != rhs.charge() ) );
}


std::ostream& Lepton::print( std::ostream& os) const{
    PhysicsObject::print( os );
    os << " / charge = " << ( _charge > 0 ? "+" : "-" );
    os << " / dxy = " << _dxy;
    os << " / dz = " << _dz;
    os << " / uncorrectedPt = " << _uncorrectedPt;
    if( generatorInfo ){
	os << " / isPrompt = " << generatorInfo->isPrompt();
	os << " / matchPdgId = " << generatorInfo->matchPdgId();
	os << " / matchCharge = " << generatorInfo->matchCharge();
    }
    return os;
}
