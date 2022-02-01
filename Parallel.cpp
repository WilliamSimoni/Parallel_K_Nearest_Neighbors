#include <iostream>
#include <vector>
#include <string.h>
#include <thread>
#include <mutex>

#include "IOUtility.hpp"
#include "utimer.cpp"

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
    if (argc != 4 && argc != 5)
        throw invalid_argument(argv[0] + string(" <file name> <K> <number of workers> [1 if you want to pin threads]"));
    string filename = argv[1];
    int K = atoi(argv[2]);
    int n_w = atoi(argv[3]);

    // read the file
    PointVector pv = read(filename);

    vector<string> results(n_w);

    // function executed by the thread
    auto f = [&](int start, int end, int id)
    {
        START(begin)

        // the string result contains the local result of the thread
        string result = "";

        for (int i = start; i < end; i++)
        {
            result += knn(pv, i, K) + "\n";
        }

        results[id] = result;
    };

    // static split and creation of the threads

    {
        utimer tm("Parallel Time");

        int chunk_size = (pv.size) / n_w;
        int bonus = (pv.size) - chunk_size * n_w;
        vector<thread> threads;

        for (int start = 0, end = chunk_size, i = 0;
             start < (pv.size);
             start = end, end = start + chunk_size, i++)
        {
            if (bonus)
            {
                end++;
                bonus--;
            }

            threads.push_back(thread(f, start, end, i));

            // pinning threads (if requested)
#ifdef DOPINNING
            // code from https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/
            //  Create a cpu_set_t object representing a set of CPUs. Clear it and mark
            //  only CPU i as set.
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            int rc = pthread_setaffinity_np(threads[i].native_handle(),
                                            sizeof(cpu_set_t), &cpuset);
            if (rc != 0)
            {
                std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
            }
#endif
        }

        for (auto &th : threads)
        {
            th.join();
        }
    }

    // save result
    save("resultParallel", results);

    return 0;
}