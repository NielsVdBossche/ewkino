#include "../interface/FourTop.h"

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
    CombinedReweighter reweighter = reweighterFactory->buildReweighter( "../weights/", yearString, treeReader->sampleVector(), btagReweighter, false );

    addBTaggingNormFactors(*btagReweighter, "ANWeights/bTagNorms/Lean");
    //std::shared_ptr<ReweighterBTagShape> btagReweighterPtr = dynamic_cast<ReweighterBTagShape*>(reweighter["bTag_shape"]);

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

        (*btagReweighter)->printNormFactors();
        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            //if (entry % 1000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry );

            currentEvent->removeTaus();
            currentEvent->selectLooseLeptons();
            currentEvent->cleanElectronsFromLooseMuons();
            currentEvent->cleanJetsFromFOLeptons();
            
            currentEvent->selectGoodJets();

            if (currentEvent->numberOfFOLeptons() != currentEvent->numberOfTightLeptons()) continue;
            currentEvent->selectTightLeptons();

            if (currentEvent->numberOfLeptons() < 2) continue;
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
            njets_NoBtagSF->Fill(currentEvent->numberOfJets(), weight_no_tag);
            yield_NoBtagSF->Fill(0., weight_no_tag);
            ht_NoBtagSF->Fill(currentEvent->HT(), weight_no_tag);

            njets->Fill(currentEvent->numberOfJets(), weight);
            yield->Fill(0., weight);
            ht->Fill(currentEvent->HT(), weight);
           

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

    }

    outfile->Close();
}
