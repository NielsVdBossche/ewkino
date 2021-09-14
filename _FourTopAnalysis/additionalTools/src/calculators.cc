#include "../interface/calculators.h"

std::vector<double> calculators::mindRInJetCollection(JetCollection& collection) {
    std::vector< std::shared_ptr< Jet > >::iterator iterOne;
    std::vector< std::shared_ptr< Jet > >::iterator iterTwo;

    std::vector<double> dRs;
    if (collection.size() < 2) return dRs;

    for (iterOne =  collection.begin(); iterOne != collection.end() - 1; iterOne++) {
        for (iterTwo = iterOne + 1; iterTwo != collection.end(); iterTwo++) {
            if (*iterOne == *iterTwo) continue;
            double dr = deltaR(**iterOne, **iterTwo);

            auto it = std::upper_bound(dRs.cbegin(), dRs.cend(), dr); //1
            dRs.insert(it, dr); //2 
        }
    }

    //std::sort(dRs.begin(), dRs.end());
    if (dRs.size() > 1) {
        if (dRs[0] > dRs[1]) {
            std::cout << "NOT SORTED: dR betw jets" << std::endl;
            exit(1);
        } 
    }
    return dRs;
}

std::vector<double> calculators::mindRLepAndJet(JetCollection& jets, LeptonCollection& leptons) {

    std::vector< std::shared_ptr< Jet > >::iterator jet;
    std::vector< std::shared_ptr< Lepton > >::iterator lep;

    std::vector<double> dRs;

    for (jet =  jets.begin(); jet != jets.end() - 1; jet++) {
        for (lep = leptons.begin(); lep != leptons.end(); lep++) {
            double dr = deltaR(**jet, **lep);

            auto it = std::upper_bound(dRs.cbegin(), dRs.cend(), dr); //1
            dRs.insert(it, dr); //2 
        }
    }


    if (dRs.size() > 1) {
        if (dRs[0] > dRs[1]) {
            std::cout << "NOT SORTED: dR betw lep and jet" << std::endl;
            exit(1);
        } 
    }

    return dRs;

}
