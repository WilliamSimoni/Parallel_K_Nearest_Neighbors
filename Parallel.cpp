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
    if (argc != 4)
        throw invalid_argument(argv[0] + string(" <file name> <K> <number of workers>"));
    string filename = argv[1];
    int K = atoi(argv[2]);
    int n_w = atoi(argv[3]);

    //read the file
    PointVector pv = read(filename);

    #ifndef NOTUSELOCALSOLUTION
    vector<string> results(n_w);

    auto f = [&](int start, int end, int id)
    {
        string result = "";

        for (int i = start; i < end; i++)
        {
            result += knn(pv, i, K) + "\n";
        }

        results[id] = result;
    };

    #else 
    vector<string> results(pv.size);

    auto f = [&](int start, int end, int id)
    {

        for (int i = start; i < end; i++)
        {
            results[i] = knn(pv, i, K);
        }
    };
    #endif

    {
        utimer tf("parallel cost");

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

        //creating threads
        for (int i = 0; i < n_w; i++)
        {
            threads.push_back(thread(f, slices[i].first, slices[i].second, i));
        }

        for (auto &th : threads)
        {
            th.join();
        }
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
}