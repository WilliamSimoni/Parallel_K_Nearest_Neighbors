#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <regex>
#include <queue>

#include "PointVector.hpp"

PointVector read(std::string filename);

void save(std::string name, std::vector<std::string> toBeSaved);

void save(std::string name, std::string toBeSaved);

std::string knn(PointVector pv, int id, int K);