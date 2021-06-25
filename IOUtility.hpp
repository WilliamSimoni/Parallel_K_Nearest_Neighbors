#include <fstream>
#include <iterator>
#include <string>
#include <vector>
#include <regex>
#include <queue>

#include "PointVector.hpp"

/**
 * read a vector of points from a file whose name is filename
 * @param filename name of the file from which read the vector of points
 * @returns a PointVector object that represents a vector of points
*/
PointVector read(std::string filename);

/**
 * save a vector of strings into a .txt file
 * @param name name of the file (without the .txt extension)
 * @param toBeSaved array of strings to be saved
 */
void save(std::string name, std::vector<std::string> toBeSaved);

/**
 * save a string into a .txt file
 * @param name name of the file (without the .txt extension)
 * @param toBeSaved string to be saved
 */ 
void save(std::string name, std::string toBeSaved);

/**
 * Given a vector of points, a point ID i and a K, it returns the K nearest neighbors of the point whose ID is i
 * @param pv vector of points
 * @param id point id 
 * @param K knn parameter
 * @return a string with the following structure: id id_k1, id_k2, id_k3, ..., id_kK where id_ki is the i-th nearest point to the point whose ID is id
 * 
 *
 */ 
std::string knn(PointVector pv, int id, int K);