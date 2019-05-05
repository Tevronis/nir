//
// Created by OKB on 05.05.2019.
//

#ifndef NIR_UTILS_H
#define NIR_UTILS_H

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

int save_vector_into_file(std::vector<std::string> &v, const std::string& filename) {
    std::ofstream outfile(filename);
    if (!outfile) {
        std::cout << "filename is not correct" << std::endl;
        raise(-1);
    }

    for (const std::string& item: v) {
        outfile << item << std::endl;
    }
    return 1;
}

std::vector<std::string> split(const std::string& str, const std::string& delim)
{
    std::vector<std::string> tokens;
    size_t prev = 0, pos = 0;
    do
    {
        pos = str.find(delim, prev);
        if (pos == std::string::npos) pos = str.length();
        std::string token = str.substr(prev, pos-prev);
        if (!token.empty()) tokens.push_back(token);
        prev = pos + delim.length();
    }
    while (pos < str.length() && prev < str.length());
    return tokens;
}



#endif //NIR_UTILS_H
