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

        priority_queue<pair<float, int>> maxheap;
        string result = "";
        
        int j = 0;
        //fill the maxheap with the first K elements
        for (j; maxheap.size() < K && j < pv.size; j++)
        {   
            if (i != j)
                maxheap.push(make_pair(pv.getDistance(i, j), j));
        }

        //continue filling
        for (j; j < pv.size; j++)
        {
            if (i == j)
                continue;
            //compute the distance
            float d = pv.getDistance(i, j);
            if (d < maxheap.top().first){
                maxheap.pop();
                maxheap.push(make_pair(d, j));
            }
        }

        //insert last element of topK in result
        result = to_string(maxheap.top().second);
        maxheap.pop();
        //insert rest of the elements
        for (int t = 1; t < K; t++){
            result = to_string(maxheap.top().second) + "," + result;
            maxheap.pop();
        }

        result = to_string(i) + "\t" + result;

        results.push_back(result);
    }

    save("result", results);
}