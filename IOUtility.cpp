#include "IOUtility.hpp"

void save(std::string name, std::vector<std::string> toBeSaved){
    std::ofstream output_file("./" + name + ".txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(toBeSaved.begin(), toBeSaved.end(), output_iterator);
    output_file.close();
}

void save(std::string name, std::string toBeSaved){
    std::ofstream output_file("./" + name + ".txt");
    output_file << toBeSaved;
    output_file.close();
}

PointVector read(std::string filename){
    // read file
    std::ifstream input(filename);
    //check if file exists
    if (input.fail())
        throw std::invalid_argument("file " + filename + " does not exist");

    //array where read data is stored
    char comma;
    float x, y;

    //PointVector where points are stored
    PointVector pv;

    // iterate over the file to populate point vector
    while ( input >> x >> comma >> y ) {
        pv.addPoint(x,y);
    }

    input.close();

    return pv;
}


std::string knn(PointVector pv, int id, int K)
{
    std::priority_queue<std::pair<float, int>> maxheap;
    std::string result = "";

    int j = 0;
    //fill the maxheap with the first K elements
    for (j; maxheap.size() < K && j < pv.size; j++)
    {
        if (id != j)
            maxheap.push(std::make_pair(pv.getDistance(id, j), j));
    }

    //continue filling
    for (j; j < pv.size; j++)
    {
        if (id == j)
            continue;
        //compute the distance
        float d = pv.getDistance(id, j);
        if (d < maxheap.top().first)
        {
            maxheap.pop();
            maxheap.push(std::make_pair(d, j));
        }
    }

    //insert last element of topK in result
    result = std::to_string(maxheap.top().second);
    maxheap.pop();
    //insert rest of the elements
    for (int t = 1; t < K; t++)
    {
        result = std::to_string(maxheap.top().second) + "," + result;
        maxheap.pop();
    }

    return std::to_string(id) + "\t" + result;
}