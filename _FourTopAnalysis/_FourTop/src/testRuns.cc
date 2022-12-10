#include "../interface/FourTop.h"

#include "../../../Tools/interface/histogramTools.h"


#if MEMLEAK
#include "../../../memleak/debug_new.h" 
#endif

void FourTop:: testRuns() {
    //ChannelManager* mgrAll = new ChannelManager(outfile, fourTopHists::testHists);
    std::shared_ptr< SampleCrossSections > xsecs;

    // reweighter creation
    std::cout << "building reweighter" << std::endl;
    std::shared_ptr< ReweighterFactory >reweighterFactory( new FourTopReweighterFactory() );
    ReweighterBTagShape** btagReweighter = new ReweighterBTagShape*();
    //CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector(), btagReweighter, false );
//
    //addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean");
    ////std::shared_ptr<ReweighterBTagShape> btagReweighterPtr = dynamic_cast<ReweighterBTagShape*>(reweighter["bTag_shape"]);

    std::cout << "event loop" << std::endl;

    std::vector<Sample> sampleVec = treeReader->sampleVector();
    std::vector<std::string> bTagShapeSystematics;

    for( unsigned sampleIndex = 0; sampleIndex < treeReader->numberOfSamples(); ++sampleIndex ){
        treeReader->initSample();
        currentEvent = treeReader->buildEventPtr(0);

        std::cerr << treeReader->currentSample().fileName() << std::endl;
        std::cout << treeReader->currentSample().fileName() << std::endl;
        TH1D* njets_NoBtagSF = new TH1D("njets_pass_noBTagSF", "njets_pass_noBTagSF;N;yield", 10, -0.5, 9.5);
        TH1D* yield_NoBtagSF = new TH1D("yield_pass_noBTagSF", "yield_pass_noBTagSF;yield;yield", 1, -0.5, 0.5);
        TH1D* ht_NoBtagSF = new TH1D("ht_pass_noBTagSF", "ht_pass_noBTagSF;yield;yield",  20, 0., 1000.);

        TH1D* njets = new TH1D("njets_pass_nom", "njets_pass_nom;N;yield", 10, -0.5, 9.5);
        TH1D* ht = new TH1D("ht_pass_nom", "ht_pass_nom;N;yield", 20, 0., 1000.);
        TH1D* yield = new TH1D("yield_pass_nom", "yield_pass_nom;yield;yield", 1, -0.5, 0.5);


        TH1D* njets_hf = new TH1D("njets_pass_hf", "njets_pass_hf;N;yield", 10, -0.5, 9.5);
        TH1D* yield_hf = new TH1D("yield_pass_hf", "yield_pass_hf;yield;yield", 1, -0.5, 0.5);
        TH1D* njets_cferr1 = new TH1D("njets_pass_cferr1", "njets_pass_cferr1;N;yield", 10, -0.5, 9.5);
        TH1D* yield_cferr1 = new TH1D("yield_pass_cferr1", "yield_pass_cferr1;yield;yield", 1, -0.5, 0.5);
        TH1D* njets_cferr2 = new TH1D("njets_pass_cferr2", "njets_pass_cferr2;N;yield", 10, -0.5, 9.5);
        TH1D* yield_cferr2 = new TH1D("yield_pass_cferr2", "yield_pass_cferr2;yield;yield", 1, -0.5, 0.5);



        TH1D* njets_hf_down = new TH1D("njets_pass_hf_down", "njets_pass_hf_down;N;yield", 10, -0.5, 9.5);
        TH1D* yield_hf_down = new TH1D("yield_pass_hf_down", "yield_pass_hf_down;yield;yield", 1, -0.5, 0.5);
        TH1D* njets_cferr1_down = new TH1D("njets_pass_cferr1_down", "njets_pass_cferr1_down;N;yield", 10, -0.5, 9.5);
        TH1D* yield_cferr1_down = new TH1D("yield_pass_cferr1_down", "yield_pass_cferr1_down;yield;yield", 1, -0.5, 0.5);
        TH1D* njets_cferr2_down = new TH1D("njets_pass_cferr2_down", "njets_pass_cferr2_down;N;yield", 10, -0.5, 9.5);
        TH1D* yield_cferr2_down = new TH1D("yield_pass_cferr2_down", "yield_pass_cferr2_down;yield;yield", 1, -0.5, 0.5);

        (*btagReweighter)->printNormFactors();
        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            //if (entry % 1000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry, false, false, false, true );

            currentEvent->removeTaus();
            currentEvent->selectLooseLeptons();
            currentEvent->cleanElectronsFromLooseMuons();
            currentEvent->cleanJetsFromFOLeptons();
            
            currentEvent->selectGoodJets();

            if (currentEvent->numberOfFOLeptons() != currentEvent->numberOfTightLeptons()) continue;
            currentEvent->selectTightLeptons();

            if (currentEvent->numberOfLeptons() != 3) continue;
            if (currentEvent->numberOfLeptons() == 2 && currentEvent->lepton(0).charge() != currentEvent->lepton(1).charge()) continue;

            if (currentEvent->numberOfJets() < 2) continue;
            if (currentEvent->numberOfLeptons() < 4 && currentEvent->HT() < 200) continue;

            double weight = currentEvent->weight();
            if( currentEvent->isMC() ){
                weight *= reweighter.totalWeight( *currentEvent );
            }

            // fill all histograms
            //eventClass nominalClass = selection->getCurrentClass();
            //if (currentEvent->numberOfJets() < 2) continue;
            //if (selection->numberOfLeps() < 4 && selection->getHT() < 200) continue;
            
            double weight_no_tag = weight / reweighter["bTag_shape"]->weight( *currentEvent );
            histogram::fillValue(njets_NoBtagSF, currentEvent->numberOfJets(), weight_no_tag);
            histogram::fillValue(yield_NoBtagSF, 0., weight_no_tag);
            histogram::fillValue(ht_NoBtagSF, currentEvent->HT(), weight_no_tag);

            histogram::fillValue(njets, currentEvent->numberOfJets(), weight);
            histogram::fillValue(yield, 0., weight);
            histogram::fillValue(ht, currentEvent->HT(), weight);


            double weight_hf= weight * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, "hf" ) / reweighter["bTag_shape"]->weight( *currentEvent );

            histogram::fillValue(njets_hf, currentEvent->numberOfJets(), weight_hf);
            histogram::fillValue(yield_hf, 0., weight_hf);

            double weight_cferr1 = weight * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, "cferr1" ) / reweighter["bTag_shape"]->weight( *currentEvent );

            histogram::fillValue(njets_cferr1, currentEvent->numberOfJets(), weight_cferr1);
            histogram::fillValue(yield_cferr1, 0., weight_cferr1);

            double weight_cferr2 = weight * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, "cferr2" ) / reweighter["bTag_shape"]->weight( *currentEvent );

            histogram::fillValue(njets_cferr2, currentEvent->numberOfJets(), weight_cferr2);
            histogram::fillValue(yield_cferr2, 0., weight_cferr2);



            double weight_hf_down= weight * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightDown( *currentEvent, "hf" ) / reweighter["bTag_shape"]->weight( *currentEvent );

            histogram::fillValue(njets_hf_down, currentEvent->numberOfJets(), weight_hf_down);
            histogram::fillValue(yield_hf_down, 0., weight_hf_down);

            double weight_cferr1_down = weight * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightDown( *currentEvent, "cferr1" ) / reweighter["bTag_shape"]->weight( *currentEvent );

            histogram::fillValue(njets_cferr1_down, currentEvent->numberOfJets(), weight_cferr1_down);
            histogram::fillValue(yield_cferr1_down, 0., weight_cferr1_down);

            double weight_cferr2_down = weight * dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightDown( *currentEvent, "cferr2" ) / reweighter["bTag_shape"]->weight( *currentEvent );

            histogram::fillValue(njets_cferr2_down, currentEvent->numberOfJets(), weight_cferr2_down);
            histogram::fillValue(yield_cferr2_down, 0., weight_cferr2_down);
        }
        
        // Output management: save histograms to a ROOT file.
        // Processes were named in samplelist. Should use this to make directory for process and subdir for sample

        // Might interface with Stacker to create desired output plots as well... Or at least already have the stacked process ready instead of individual components. Then a "getDirectory" in stacker could be handy to see if it exists.
        std::cout << "output" << std::endl;
        
        outfile->cd();

        njets_NoBtagSF->Write(njets_NoBtagSF->GetName(), TObject::kOverwrite);
        yield_NoBtagSF->Write(yield_NoBtagSF->GetName(), TObject::kOverwrite);
        ht_NoBtagSF->Write(ht_NoBtagSF->GetName(), TObject::kOverwrite);
        njets->Write(njets->GetName(), TObject::kOverwrite);
        yield->Write(yield->GetName(), TObject::kOverwrite);
        ht->Write(ht->GetName(), TObject::kOverwrite);
        njets_hf->Write(njets_hf->GetName(), TObject::kOverwrite);
        yield_hf->Write(yield_hf->GetName(), TObject::kOverwrite);
        njets_cferr1->Write(njets_cferr1->GetName(), TObject::kOverwrite);
        yield_cferr1->Write(yield_cferr1->GetName(), TObject::kOverwrite);
        njets_cferr2->Write(njets_cferr2->GetName(), TObject::kOverwrite);
        yield_cferr2->Write(yield_cferr2->GetName(), TObject::kOverwrite);

        njets_hf_down->Write(njets_hf_down->GetName(), TObject::kOverwrite);
        yield_hf_down->Write(yield_hf_down->GetName(), TObject::kOverwrite);
        njets_cferr1_down->Write(njets_cferr1_down->GetName(), TObject::kOverwrite);
        yield_cferr1_down->Write(yield_cferr1_down->GetName(), TObject::kOverwrite);
        njets_cferr2_down->Write(njets_cferr2_down->GetName(), TObject::kOverwrite);
        yield_cferr2_down->Write(yield_cferr2_down->GetName(), TObject::kOverwrite);
    }

    outfile->Close();
}
