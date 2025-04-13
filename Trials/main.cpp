//
// Created by kubam on 12.03.2025.
//
#include <chrono>
#include <iostream>

#include "Packer.h"
#include "Trials.h"


int main() {
    for (int i = 1; i <= 10; i++) {
        auto data=read_file("BR data set\\BR"+std::to_string(i)+".txt");
        long long total_time=0;
        double total_container_usage=0;
        for (const auto& [container,boxes]:data) {
            Packer packer(boxes,container);
            auto start = std::chrono::high_resolution_clock::now();
            auto results=packer.pack();
            auto stop = std::chrono::high_resolution_clock::now();
            total_time += std::chrono::duration_cast<std::chrono::microseconds>(stop - start).count();
            total_container_usage+=calculate_container_usage(container,results);
        }
        std::cout<<"BR"<<i<<" result: average time: "<< total_time/data.size()<<" µs average usage: "<<total_container_usage/data.size()<<'\n';

    }
    return 0;
}