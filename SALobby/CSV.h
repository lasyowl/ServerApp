#pragma once

#include <list>
#include <unordered_map>
#include <fstream>

namespace CSV {
	std::list<std::unordered_map<std::string, std::string> > Load( std::string sFilePath );
}