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
    std::string line;

    //PointVector where points are stored
    PointVector pv;

    // Use a while loop together with the getline() function to read the file line by line
    std::regex reg(",");
    while (getline(input, line))
    {
        //tokenizing the line
        std::sregex_token_iterator iter(line.begin(), line.end(), reg, -1);
        std::sregex_token_iterator end;
        std::vector<std::string> vec(iter, end);

        //creating the point
        Point tmp;
        tmp.x = stof(vec[0]);
        tmp.y = stof(vec[1]);

        //inserting the point
        pv.addPoint(tmp);
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