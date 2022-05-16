#include "csvreader.h"

#include <iostream>
#include <fstream>
#include <sstream>


std::vector<std::vector<double>> LoadCsvLog(std::string file_path, int skip_rows) {
    std::ifstream ifs(file_path);
    std::vector<std::vector<double>> res_vectors;
    std::string line;

    std::getline(ifs, line);
    auto str = split(line, ',');
    int n_cols = str.size();
    for (int i=0; i < n_cols; ++i) {
        std::vector<double> temp1;
        res_vectors.push_back(temp1);
    }
    ifs.seekg(0, std::ios_base::beg);

    // skip rows
    for (int i=0; i < skip_rows; ++i) std::getline(ifs, line);

    while (std::getline(ifs, line)) {
        auto str = split(line, ',');
        for (int i=0; i < n_cols; ++i) {
            res_vectors[i].push_back(std::stod(str[i]));
        }
    }
    return res_vectors;
};

std::vector<std::string> split(std::string& line, char delimiter) {
    std::istringstream stream(line);
    std::string field;
    std::vector<std::string> res;
    while (getline(stream, field, delimiter)) {
        res.push_back(field);
    }
    return res;
};
