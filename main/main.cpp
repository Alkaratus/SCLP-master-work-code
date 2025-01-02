//
// Created by kubam on 07.02.2024.
//

#include <iostream>

#include "Box_Generators.h"
#include "Container.h"
#include "Packer.h"
#include "Insertion_Coordinates_Displayer.h"
#include <fstream>

int main(int argc, char*argv[]){
    Container container(10,10,10);
    auto boxes= generate_boxes(container.get_volume(),geometrical_distribution,5);
    Packer packer(boxes,container);
    auto packing_result=packer.pack();
    std::fstream file("coordinates.json",std::ios::out);
    file<<Insertion_Coordinates_Displayer().display_all_coordinates_in_list(packing_result);
    std::cout<<calculate_container_usage(container,packing_result);
    return 0;
}