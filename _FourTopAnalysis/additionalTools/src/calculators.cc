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

    for (jet =  jets.begin(); jet != jets.end(); jet++) {
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

std::pair<std::shared_ptr<Jet>, std::shared_ptr<Lepton>> calculators::closestLepAndJet(JetCollection& jets, LeptonCollection& leptons) {
    // calculate which are closest

    std::vector< std::shared_ptr< Jet > >::iterator jet;
    std::vector< std::shared_ptr< Lepton > >::iterator lep;

    double dr = 10.;
    std::pair<std::shared_ptr<Jet>, std::shared_ptr<Lepton>> closestPair;

    for (jet =  jets.begin(); jet != jets.end(); jet++) {
        for (lep = leptons.begin(); lep != leptons.end(); lep++) {
            double drNew = deltaR(**jet, **lep);
            
            if (drNew < dr) {
                dr = drNew;
                closestPair = {*jet, *lep};
            }
        }
    }

    return closestPair;
}


std::vector<double> calculators::mindRLeptons(LeptonCollection& leptons) {
    std::vector< std::shared_ptr< Lepton > >::iterator iterOne;
    std::vector< std::shared_ptr< Lepton > >::iterator iterTwo;

    std::vector<double> dRs;
    if (leptons.size() < 2) return dRs;

    for (iterOne =  leptons.begin(); iterOne != leptons.end() - 1; iterOne++) {
        for (iterTwo = iterOne + 1; iterTwo != leptons.end(); iterTwo++) {
            if (*iterOne == *iterTwo) continue;
            double dr = deltaR(**iterOne, **iterTwo);

            auto it = std::upper_bound(dRs.cbegin(), dRs.cend(), dr); //1
            dRs.insert(it, dr); //2 
        }
    }

    //std::sort(dRs.begin(), dRs.end());
    if (dRs.size() > 1) {
        if (dRs[0] > dRs[1]) {
            std::cout << "NOT SORTED: dR betw leptons" << std::endl;
            exit(1);
        } 
    }
    return dRs;
}

std::vector<double> calculators::mindEtaLeptons(LeptonCollection& leptons) {
    std::vector< std::shared_ptr< Lepton > >::iterator iterOne;
    std::vector< std::shared_ptr< Lepton > >::iterator iterTwo;

    std::vector<double> dRs;
    if (leptons.size() < 2) return dRs;

    for (iterOne =  leptons.begin(); iterOne != leptons.end() - 1; iterOne++) {
        for (iterTwo = iterOne + 1; iterTwo != leptons.end(); iterTwo++) {
            if (*iterOne == *iterTwo) continue;
            double dr = deltaEta(**iterOne, **iterTwo);

            auto it = std::upper_bound(dRs.cbegin(), dRs.cend(), dr); //1
            dRs.insert(it, dr); //2 
        }
    }

    //std::sort(dRs.begin(), dRs.end());
    if (dRs.size() > 1) {
        if (dRs[0] > dRs[1]) {
            std::cout << "NOT SORTED: dEta betw leptons" << std::endl;
            exit(1);
        } 
    }
    return dRs;
}

std::vector<double> calculators::mindPhiLeptons(LeptonCollection& leptons) {
    std::vector< std::shared_ptr< Lepton > >::iterator iterOne;
    std::vector< std::shared_ptr< Lepton > >::iterator iterTwo;

    std::vector<double> dRs;
    if (leptons.size() < 2) return dRs;

    for (iterOne =  leptons.begin(); iterOne != leptons.end() - 1; iterOne++) {
        for (iterTwo = iterOne + 1; iterTwo != leptons.end(); iterTwo++) {
            if (*iterOne == *iterTwo) continue;
            double dr = deltaPhi(**iterOne, **iterTwo);

            auto it = std::upper_bound(dRs.cbegin(), dRs.cend(), dr); //1
            dRs.insert(it, dr); //2 
        }
    }

    //std::sort(dRs.begin(), dRs.end());
    if (dRs.size() > 1) {
        if (dRs[0] > dRs[1]) {
            std::cout << "NOT SORTED: dPhi betw leptons" << std::endl;
            exit(1);
        } 
    }
    return dRs;
}

