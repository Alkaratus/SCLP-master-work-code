//
// Created by kubam on 12.03.2025.
//

#include <fstream>
#include "Trials.h"

std::string get_word(std::istream& stream) {
    std::string word;
    stream>>word;
    return word;
}

std::vector<std::pair<Container,std::list<Box>>> read_file(const std::string& filename) {
    std::fstream file(filename,std::ios::in);
    if (!file.good()) {
        throw std::invalid_argument("File not found");
    }
    auto number_of_pairs= std::stoul(get_word(file));
    std::vector<std::pair<Container,std::list<Box>>> data;

    for(unsigned int i=0;i< number_of_pairs;i++) {
        get_word(file);get_word(file);
        Container container(std::stoul(get_word(file)),std::stoul(get_word(file)),std::stoul(get_word(file)));
        std::list<Box> boxes;
        auto const number_of_boxes_types=std::stoul(get_word(file));
        for(unsigned int j=0;j<number_of_boxes_types;j++) {
            get_word(file);
            auto width=std::stoul(get_word(file));
            get_word(file);
            auto depth=std::stoul(get_word(file));
            get_word(file);
            auto height=std::stoul(get_word(file));
            get_word(file);
            auto quantity=std::stoul(get_word(file));
            for(unsigned int k=0;k<quantity;k++) {
                boxes.emplace_back(width,depth,height);
            }
        }
        data.emplace_back(container,boxes);
    }
    return data;
}



