#include <iostream>
#include <fstream>
#include <vector>
#include <regex>
#include <string.h>
#include <queue>

#include "IOUtility.hpp"

using namespace std;

int main(int argc, char *argv[])
{
    /*
        filename: file name from which read knn input
    */

    // read program parameters
    if (argc != 3)
        throw invalid_argument(argv[0] + string(" <file name> <K>"));
    string filename = argv[1];
    int K = atoi(argv[2]);

    //read the file
    PointVector pv = read(filename);
    //Where the program stores the knn results for each point
    vector<string> results; 
    //write header 
    results.push_back("ID\t KNN");

    //kmean
    for (int i = 0; i < pv.size; i++)
    {
        results.push_back(knn(pv,i,K));
    }

    save("result", results);
}