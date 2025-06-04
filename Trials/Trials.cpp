//
// Created by kubam on 12.03.2025.
//

#include <fstream>
#include <chrono>
#include <iostream>
#include <sstream>
#include "Trials.h"

#include "Insertion_Coordinates_Displayer.h"
#include "Packer.h"
#include "Scenerio_Tree_Packer.h"

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
    data.reserve(number_of_pairs);
    for(unsigned int i=0;i< number_of_pairs;i++) {
        data.emplace_back(read_pair(file));
    }
    return data;
}

std::pair<Container, std::list<Box>> read_pair(std::istream &stream) {
    get_word(stream);get_word(stream);
    Container container(std::stoul(get_word(stream)),std::stoul(get_word(stream)),
        std::stoul(get_word(stream)),Container::manhattan_distance);
    std::list<Box> boxes;
    auto const number_of_boxes_types=std::stoul(get_word(stream));
    for(unsigned int j=0;j<number_of_boxes_types;j++) {
        get_word(stream);
        auto width=std::stoul(get_word(stream));
        get_word(stream);
        auto depth=std::stoul(get_word(stream));
        get_word(stream);
        auto height=std::stoul(get_word(stream));
        get_word(stream);
        auto quantity=std::stoul(get_word(stream));
        for(unsigned int k=0;k<quantity;k++) {
            boxes.emplace_back(width,depth,height);
        }
    }
    return {container,boxes};
}

void simple_packer_trial() {
    for (int i = 1; i <= 10; i++) {
        auto data=read_file("BR data set\\br"+std::to_string(i)+".txt");
        long long total_time=0;
        double total_container_usage=0;
        int j=0;
        for (const auto& [container,boxes]:data) {
            Packer packer(boxes,container,Packer::by_max_volume);
            auto start = std::chrono::high_resolution_clock::now();
            auto results=packer.pack();
            auto stop = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
            auto container_usage=calculate_container_usage(container,results);
            if(container_usage > 1) {
                throw std::invalid_argument("Overload container usage, set: "+std::to_string(i)+" iteration: "+std::to_string(j));
            }
            total_container_usage+=container_usage;
            ++j;
        }
        std::cout<<"BR"<<i<<" result: average time: "<< total_time/data.size()<<" qs average usage: "<<total_container_usage/data.size()<<std::endl;

    }
}

void tree_packer_trial() {
    for (int i = 1; i <= 10; i++) {
        auto data=read_file("BR data set\\br"+std::to_string(i)+".txt");
        long long total_time=0;
        double total_container_usage=0;
        int j=0;
        for (const auto& [container,boxes]:data) {
            Scenerio_Tree_Packer packer(boxes,container,2,{2,2});
            auto start = std::chrono::high_resolution_clock::now();
            auto results=packer.pack();
            auto stop = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
            auto container_usage=calculate_container_usage(container,results);
            if(container_usage > 1) {
                throw std::invalid_argument("Overload container usage, set: "+std::to_string(i)+" iteration: "+std::to_string(j));
            }
            total_container_usage+=container_usage;
            ++j;
        }
        std::cout<<"BR"<<i<<" result: average time: "<< total_time/data.size()<<" qs average usage: "<<total_container_usage/data.size()<<std::endl;

    }
}

void mock_up_trial() {
    std::stringstream pair_data;
    pair_data << "1 2502505 587 233 220 3 1 108 0 76 0 30 1 40 2 110 0 43 1 25 1 33 3 92 1 81 1 55 1 39";
    auto [container,boxes]=read_pair(pair_data);
    Scenerio_Tree_Packer packer(boxes,container,2,{2,2});
    auto results=packer.pack();
    auto container_usage=calculate_container_usage(container,results);
    if(container_usage > 1) {
        throw std::invalid_argument("Overload container usage");
    }
}

void compare_packers(int file_index, int dataset) {
    auto data=read_file("BR data set\\br"+std::to_string(file_index)+".txt");
    auto [container,boxes]=data[dataset];
    Packer packer(boxes,container,Packer::by_max_volume);
    Scenerio_Tree_Packer tree_packer(boxes,container,2,{2,2});
    auto packer_results=packer.pack();
    auto tree_packer_results=tree_packer.pack();
    std::cout<<"Simple Packer effectiveness: "<<calculate_container_usage(container,packer_results)<<"\n";
    std::cout<<"Tree Packer effectiveness: "<<calculate_container_usage(container,tree_packer_results)<<"\n";
    std::cout<<Insertion_Coordinates_Displayer().display_all_coordinates_in_list(packer_results);
    std::cout<<"\n";
    std::cout<<Insertion_Coordinates_Displayer().display_all_coordinates_in_list(tree_packer_results);
}


void simple_packer_trial(int file_index, int dataset) {
    auto data=read_file("BR data set\\br"+std::to_string(file_index)+".txt");
    auto [container,boxes]=data[dataset];
    Packer packer(boxes,container,Packer::by_max_volume);
    auto packer_results=packer.pack();
    std::cout<<"Simple Packer effectiveness: "<<calculate_container_usage(container,packer_results)<<"\n";
    std::cout<<Insertion_Coordinates_Displayer().display_all_coordinates_in_list(packer_results);
}

void tree_packer_trial(int file_index, int dataset) {
    auto data=read_file("BR data set\\br"+std::to_string(file_index)+".txt");
    auto [container,boxes]=data[dataset];
    Scenerio_Tree_Packer tree_packer(boxes,container,2,{2,2});
    auto tree_packer_results=tree_packer.pack();
    std::cout<<"Tree Packer effectiveness: "<<calculate_container_usage(container,tree_packer_results)<<"\n";
    std::cout<<Insertion_Coordinates_Displayer().display_all_coordinates_in_list(tree_packer_results);
}