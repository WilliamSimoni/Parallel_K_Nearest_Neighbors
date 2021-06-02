#include "IOUtility.hpp"

void save(std::string name, std::vector<std::string> toBeSaved){
    std::ofstream output_file("./" + name + ".txt");
    std::ostream_iterator<std::string> output_iterator(output_file, "\n");
    std::copy(toBeSaved.begin(), toBeSaved.end(), output_iterator);
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