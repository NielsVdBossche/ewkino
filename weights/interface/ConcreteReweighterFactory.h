/*
New user specified ReweighterFactory derivates should be listed here
*/

#ifndef ConcreteReweighterFactory_H
#define ConcreteReweighterFactory_H

#include "ReweighterFactory.h"

class ReweighterBTagShape;

class EmptyReweighterFactory : public ReweighterFactory {
    public: 
	virtual CombinedReweighter buildReweighter( const std::string&, const std::string&, const std::vector< Sample >& ) const override;
};


class EwkinoReweighterFactory : public ReweighterFactory {

    public:
        virtual CombinedReweighter buildReweighter( const std::string&, const std::string&, const std::vector< Sample >& ) const override;
};

class FourTopReweighterFactory : public ReweighterFactory {
    public:
        virtual CombinedReweighter buildReweighter( const std::string&, const std::string&, const std::vector< Sample >& ) const override;
        virtual CombinedReweighter buildReweighter( const std::string&, const std::string&, const std::vector< Sample >& , ReweighterBTagShape**, bool) const override;
};

#endif
