#ifndef JetInfo_H
#define JetInfo_H

// include c++ library classes
#include <map>
#include <string>
#include <algorithm>


class JetInfo{

    public: 

	JetInfo( const TreeReader&, 
		 const bool readAllJECVariations = false, 
		 const bool readGroupedJECVariations = false );
	std::vector< std::string > allJECVariations(){ return _JECSources; }
	std::vector< std::string > groupedJECVariations(){ return _JECGrouped; }

	std::map< std::string, size_t >* allJECVariationsMap(){ return _sourcesJEC_Ids; }
	std::map< std::string, size_t >* groupedJECVariationsMap(){ return _groupedJEC_Ids; }

	bool hasJECVariation( const std::string& ) const;
	void printAllJECVariations() const;
	void printGroupedJECVariations() const;

    private:

	std::vector< std::string > _JECSources;
	std::vector< std::string > _JECGrouped;

    std::map< std::string, size_t >* _groupedJEC_Ids;
    std::map< std::string, size_t >* _sourcesJEC_Ids;
};

#endif
