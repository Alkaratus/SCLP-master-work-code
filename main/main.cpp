//
// Created by kubam on 07.02.2024.
//

#include <iostream>

#include "Box_Generators.h"
#include "Container.h"
#include "Packer.h"

int main(int argc, char*argv[]){
    Container container(10,10,10);
    auto boxes= generate_boxes(container.get_volume(),geometrical_distribution,5);
    Packer packer(boxes,container);
    auto packing_result=packer.pack();
    //std::cout<<calculate_container_usage(container,packing_result);
    for(const auto &result:packing_result) {
        std::cout<<result->create_json();
    }
    return 0;
}