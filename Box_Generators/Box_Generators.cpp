//
// Created by kubam on 04.03.2024.
//

#include "Box_Generators.h"
#include <random>
#include <cmath>

std::list<Box>generate_boxes(unsigned int volume, unsigned int method()){
    std::list<Box>generated_boxes;
    bool overflow=false;
    while(!overflow){
        auto &&new_box=generate_box(method);
        overflow=volume<new_box.get_volume();
        volume-=new_box.get_volume();
        generated_boxes.emplace_back(new_box);
    }
    return generated_boxes;
}

std::list<Box>generate_boxes(unsigned int volume,unsigned int method(unsigned int), unsigned int max_size){
    std::list<Box>generated_boxes;
    bool overflow=false;
    while(!overflow){
        auto &&new_box=generate_box(method,max_size);
        overflow=volume<new_box.get_volume();
        volume-=new_box.get_volume();
        generated_boxes.emplace_back(new_box);
    }
    return generated_boxes;
}

Box generate_box(unsigned int method()) {
    return {method(), method(), method()};
}

Box generate_box(unsigned int method(unsigned int), unsigned int max_value) {
    return {method(max_value), method(max_value), method(max_value)};
}

unsigned int geometrical_distribution(){
    const double Q=0.8;
    static std::default_random_engine engine;
    static std::uniform_real_distribution<double> distribution(0,1);
    auto value=distribution(engine);
    value=1-value;
    return ceil(log(value)/log(Q));
}

unsigned int geometrical_distribution(unsigned int max_value) {
    const double Q=0.8;
    static std::default_random_engine engine;
    auto cumulative_distribution=1-pow(Q,max_value)/Q;
    static std::uniform_real_distribution<double> distribution(0,cumulative_distribution);
    auto value=distribution(engine);
    value=1-value;
    return ceil(log(value)/log(Q));
}



