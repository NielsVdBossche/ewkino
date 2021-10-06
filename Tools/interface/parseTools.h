#ifndef PARSETOOLS_H
#define PARSETOOLS_H

#include <string>
#include <utility>

// methods to help parse txt files to settings/pathnames/...

namespace parseTools {

    bool skipLine(const std::string& line);
    std::pair<std::string, std::string> splitNameAndValue(std::string& line, char delim='=');
}


#endif