#include <iostream>
#include <vector>
#include <string.h>
#include <thread>

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
    //Where the program stores the knn results for each point
    vector<string> results(pv.size);

    // static split
    int chunk_size = (results.size()) / n_w;
    int bonus = (results.size()) - chunk_size * n_w;
    vector<pair<float, int>> slices;

    for (int start = 0, end = chunk_size;
         start < (results.size());
         start = end, end = start + chunk_size)
    {
        if (bonus)
        {
            end++;
            bonus--;
        }

        slices.push_back(make_pair(start, end));
    }

    auto f = [&](int start, int end)
    {
        for (vector<string>::iterator it = results.begin() + start; it != results.begin() + end; ++it)
        {
            *it = knn(pv, distance(results.begin(), it), K);
        }
    };

    vector<thread> threads;

    {
        utimer tf("Non serial fraction");

        //creating threads
        for (int i = 0; i < n_w; i++)
        {
            threads.push_back(thread(f, slices[i].first, slices[i].second));
        }

        for (auto &th : threads)
        {
            th.join();
        }
    }

    results.insert(results.begin(), "ID\t KNN");

    save("result", results);
}