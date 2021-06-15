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

    //read the file
    PointVector pv = read(filename);
    //Where the program stores the knn results for each point
    vector<string> results(pv.size);

    auto f = [&](const long i)
    {
        results[i] = knn(pv, i, K);
    };

    ParallelFor pf(n_w);
    {
        utimer tf("Non serial fraction");
        pf.parallel_for(0, results.size(), 1, 0, f);
    }

    results.insert(results.begin(), "ID\t KNN");

    save("result", results);
    return 0;
}