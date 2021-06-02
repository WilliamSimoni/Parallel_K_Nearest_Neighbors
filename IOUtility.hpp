#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <regex>

#include "PointVector.hpp"

PointVector read(std::string filename);

void save(std::string name, std::vector<std::string> toBeSaved);