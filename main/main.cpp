//
// Created by kubam on 07.02.2024.
//

#include <iostream>

#include "Box_Generators.h"
#include "Container.h"
#include "Packer.h"
#include "Scenerio_Tree_Packer.h"
#include "Insertion_Coordinates_Displayer.h"
#include <fstream>

#include "Visualisation_Coordinates_Displayer.h"
#include "Packer_Parser.h"

int main(int argc, char*argv[]){

    auto generated_packer=generate_packer("input.json");

    Container container(10,10,10);
    auto boxes= generate_boxes(container.get_volume(),geometrical_distribution,5);
    //Packer packer(boxes,container);
    Scenerio_Tree_Packer packer(boxes,container,2,{2,2});
    auto packing_result=packer.pack();
    std::fstream file("app_coordinates.json",std::ios::out);
    file<<Visualisation_Coordinates_Displayer().get_data_for_visualisation(&packer,packing_result);
    std::cout<<calculate_container_usage(container,packing_result);
    return 0;
}
