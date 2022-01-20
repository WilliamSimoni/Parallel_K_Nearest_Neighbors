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

    int pin_threads = 0;
    if (argc == 5)
        pin_threads = atoi(argv[4]);

    // read the file
    PointVector pv = read(filename);

    // vector containing the time required by each worker
    vector<long int> times_per_thread(n_w);

#ifndef NOTUSELOCALSOLUTION
    vector<string> results(n_w);

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

        STOP(begin, elapsed)
        times_per_thread[id] = elapsed;
    };

#else

    vector<string> results(pv.size);

    auto f = [&](int start, int end, int id)
    {
        START(begin)

        for (int i = start; i < end; i++)
        {
            results[i] = knn(pv, i, K);
        }

        STOP(begin, elapsed)
        times_per_thread[id] = elapsed;
    };
#endif

    // static split
    int chunk_size = (pv.size) / n_w;
    int bonus = (pv.size) - chunk_size * n_w;
    vector<pair<float, int>> slices;
    for (int start = 0, end = chunk_size;
         start < (pv.size);
         start = end, end = start + chunk_size)
    {
        if (bonus)
        {
            end++;
            bonus--;
        }

        slices.push_back(make_pair(start, end));
    }

    vector<thread> threads;

    // creating threads
    for (int i = 0; i < n_w; i++)
    {
        threads.push_back(thread(f, slices[i].first, slices[i].second, i));

        //pinning threads (if requested)
        if (pin_threads == 1)
        {
            //code from https://eli.thegreenplace.net/2016/c11-threads-affinity-and-hyperthreading/
            // Create a cpu_set_t object representing a set of CPUs. Clear it and mark
            // only CPU i as set.
            cpu_set_t cpuset;
            CPU_ZERO(&cpuset);
            CPU_SET(i, &cpuset);
            int rc = pthread_setaffinity_np(threads[i].native_handle(),
                                            sizeof(cpu_set_t), &cpuset);
            if (rc != 0)
            {
                std::cerr << "Error calling pthread_setaffinity_np: " << rc << "\n";
            }
        }
    }

    for (auto &th : threads)
    {
        th.join();
    }

#ifndef NOTUSELOCALSOLUTION
    ofstream output_file("./resultParallel.txt");
    for (int i = 0; i < n_w; i++)
    {
        output_file << results[i];
    }
    output_file.close();

#else

    save("./resultParallel", results);

#endif

    // computing tot time for parallel part
    auto max_time = times_per_thread[0];
    for (int i = 1; i < n_w; i++)
    {
        if (max_time < times_per_thread[i])
            max_time = times_per_thread[i];
    }

    // print the parallel time (max of times_per_thread)
    cout << "parallel cost computed in " << max_time << " usec " << std::endl;

    return 0;
}