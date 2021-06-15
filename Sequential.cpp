#include <iostream>
#include <vector>
#include <string.h>

#include "IOUtility.hpp"
#include "utimer.cpp"

using namespace std;

int main(int argc, char *argv[])
{
    /*
        filename: file name from which read knn input
        K: knn parameter
    */

    utimer te("Entire Execution");

    // read program parameters
    if (argc != 3)
        throw invalid_argument(argv[0] + string(" <file name> <K>"));
    string filename = argv[1];
    int K = atoi(argv[2]);

    /*SERIAL FRACTION*/
    //read the file

    PointVector pv = read(filename);
    //Where the program stores the knn results for each point
    vector<string> results;
    //write header
    results.push_back("ID\t KNN");

    /*NON SERIAL FRACTION*/
    //kmean
    {   
        utimer tf("Non serial fraction");
        for (int i = 0; i < pv.size; i++)
        {
            results.push_back(knn(pv, i, K));
        }
    }

    /*SERIAL FRACTION*/
    save("result", results);

    //max_speedup = 1/(Entire Execution time - Non serial fraction time)/Entire Execution time
}