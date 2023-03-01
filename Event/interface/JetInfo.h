#ifndef JetInfo_H
#define JetInfo_H

// include c++ library classes
#include <map>
#include <string>
#include <algorithm>


class JetInfo{

    public: 

	JetInfo( const TreeReader& );
	std::vector< std::string > allJECVariations(){ return _JECSources; }
	std::vector< std::string > groupedJECVariations(){ return _JECGrouped; }

	bool hasJECVariation( const std::string& ) const;
	void printAllJECVariations() const;
	void printGroupedJECVariations() const;

    private:

	std::vector< std::string > _JECSources;
	std::vector< std::string > _JECGrouped;
};

#endif
