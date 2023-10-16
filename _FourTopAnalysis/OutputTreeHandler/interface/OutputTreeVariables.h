#ifndef FOURTOP_OUTPUTTREE_VARIABLES_H
#define FOURTOP_OUTPUTTREE_VARIABLES_H

#include "OutputTree.h"
#include "../../FourTopEventHandling/interface/EventFourT.h"

class OutputTreeVariables : public OutputTree {
    public:
        OutputTreeVariables(TFile* outputfile, std::string& treeName, std::string& outputLevel);
        ~OutputTreeVariables();
        
        virtual void FillTree(EventFourT*, double) = 0;
        void FillBaseTree(double weight, EventFourT* ftEvent);
        unsigned GetOutputInt() const {return outputInt;}

        // Base variables
        double nominalWeight;
        double noSFWeight;
        
        unsigned nJets;
        std::vector<double> jetPt; // only tight jets

        // potentially change this to just have the nBTight/med/loose? Rather than wp based, then again count 3/2/1/0 is also possible
        std::vector<int> bTagWP; // 0=no, 1=loose, 2=med, 3=tight?

        unsigned nElectrons;
        std::vector<double> electronPt;
        std::vector<int> electronCharge;
        std::vector<bool> isElectron;

        unsigned nMuons;
        std::vector<double> muonPt;
        std::vector<int> muonCharge;


        double ptMiss;

        double fourTopScore;
        double ttWScore;
        double ttbarScore;

        unsigned eventClass;

        double invariant_mass_ll;

        // BDT variables
        double min_dR_bb;
        double dR_l1l2;
        double dPhi_l1l2;
        double dEta_l1l2;
        double min_dR_ll;
        double min_dPhi_ll;
        double min_dEta_ll;

        double max_mOverPt;
        double min_dR_lepB;
        double minTwo_dR_lepB;

        double max_DF_1;
        double max_DF_2;
        double max_DF_3;
        double max_DF_4;

        double df_j1;
        double df_j2;
        double df_j3;
        double df_j4;

        double mtop1, mtop2;
        double mW1, mW2;

        double mt_l1;
        double mt_l2;
        double mt2_ll;
        double mt2_bb;
        double mt2_lblb;

        double m_l1l2;
        double m_l2l3;
        double m_l1l3;
        // bdt variables? Put something that decides to initialize
    private:
        std::string outputLevel;
        unsigned outputInt;
        
        void pInitTree();
};


#endif