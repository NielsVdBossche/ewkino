#include "../interface/parseTools.h"

bool parseTools::skipLine(const std::string& line) {
    if (line.find_first_not_of( " \t" ) == std::string::npos) return true;
    if (line.at(line.find_first_not_of( " \t" )) == '#') return true;

    return false;
}

std::pair<std::string, std::string> parseTools::splitNameAndValue(std::string& line, char delim) {
    int pos = line.find(delim);
    
    return {line.substr(0, pos), line.substr(pos+1, line.size())};
}
