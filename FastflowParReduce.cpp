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
    string result = "";

    auto map_f = [&](const long i, string& loc_res)
    {
        loc_res += knn(pv, i, K) + "\n";
    };

    auto reduce_f = [](string &res, string loc_res){
        res += loc_res;
    };

    // call parallel_for_idx in order to have id of the thread
    {
        utimer tp("Parallel Time");
        ParallelForReduce<string> pfr(n_w);
        pfr.parallel_reduce(result, "", 0, pv.size, 1, 0, map_f, reduce_f, n_w);
    }

    // save result
    save("resultFastFlow", result);

    return 0;
}