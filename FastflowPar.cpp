#include <ff/parallel_for.hpp>
#include <iostream>
#include <vector>
#include <string.h>
#include <thread>

#include "IOUtility.hpp"
#include "utimer.cpp"

using namespace ff;

using namespace std;

int main(int argc, char *argv[])
{
    /*
        filename: file name from which read knn input
        K: knn parameter
        n_w: number of threads
    */

    utimer te("Entire Execution");

    // read program parameters
    if (argc != 4)
        throw invalid_argument(argv[0] + string(" <file name> <K> <number of workers>"));
    string filename = argv[1];
    int K = atoi(argv[2]);
    int n_w = atoi(argv[3]);

    // read the file
    PointVector pv = read(filename);

    // Where the program stores the knn results for each point
    vector<string> results(n_w);

    // vector containing the time required by each worker
    vector<long int> times_per_thread(n_w);

    auto f = [&](const long start, const long stop, const long thid)
    {
        // the string result contains the local result of the thread
        string result = "";

        for (int i = start; i < stop; i++)
        {
            result += knn(pv, i, K) + "\n";
        }

        results[thid] = result;
    };

    // call parallel_for_idx in order to have id of the thread
    {
        utimer tp("Parallel Time");
        parallel_for_idx(0, pv.size, 1, 0, f, n_w);
    }

    // save result
    save("resultFastFlow", results);
    
    return 0;
}