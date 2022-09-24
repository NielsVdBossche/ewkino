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

        TH1D* njets = new TH1D("njets_pass_nom", "njets_pass_nom;N;yield", 10, -0.5, 9.5);
        TH1D* njets_var = new TH1D("njets_pass_reweight", "njets_pass_reweight;N;yield", 10, -0.5, 9.5);
        TH1D* yield = new TH1D("yield_pass_nom", "yield_pass_nom;yield;yield", 1, -0.5, 0.5);
        TH1D* yield_var = new TH1D("yield_pass_reweight", "yield_pass_reweight;yield;yield", 1, -0.5, 0.5);

        for( long unsigned entry = 0; entry < treeReader->numberOfEntries(); ++entry ){
            //if (entry > 10000) break;
            //if (entry % 1000 == 0) std::cout << entry << "/" << treeReader->numberOfEntries() << std::endl;
            delete currentEvent;

            // Initialize event
            currentEvent = treeReader->buildEventPtr( entry );
            selection->addNewEvent(currentEvent);

            if (! eventPassesTriggers()) continue;

            if (! selection->passLeptonSelection()) continue;
            selection->classifyEvent();

            double weight = currentEvent->weight();
            if( currentEvent->isMC() ){
                weight *= reweighter.totalWeight( *currentEvent );
            }

            // fill all histograms
            //eventClass nominalClass = selection->getCurrentClass();
            if (selection->numberOfJets() < 2) continue;
            if (selection->numberOfLeps() && selection->getHT() < 200) continue;

            njets->Fill(selection->numberOfJets(), weight);
            yield->Fill(0., weight);
           
            weight *= dynamic_cast<const ReweighterBTagShape*>(reweighter["bTag_shape"])->weightUp( *currentEvent, "lf" ) / reweighter["bTag_shape"]->weight( *currentEvent );

            njets_var->Fill(selection->numberOfJets(), weight);
            yield_var->Fill(0., weight);
        }
        
        // Output management: save histograms to a ROOT file.
        // Processes were named in samplelist. Should use this to make directory for process and subdir for sample

        // Might interface with Stacker to create desired output plots as well... Or at least already have the stacked process ready instead of individual components. Then a "getDirectory" in stacker could be handy to see if it exists.
        std::cout << "output" << std::endl;
        
        outfile->cd();
        njets->Write(njets->GetName(), TObject::kOverwrite);
        yield->Write(yield->GetName(), TObject::kOverwrite);
        njets_var->Write(njets_var->GetName(), TObject::kOverwrite);
        yield_var->Write(yield_var->GetName(), TObject::kOverwrite);
    }

    outfile->Close();
}
