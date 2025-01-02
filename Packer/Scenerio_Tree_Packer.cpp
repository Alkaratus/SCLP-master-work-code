//
// Created by kubam on 16.12.2024.
//

#include "Scenerio_Tree_Packer.h"

#include <utility>

using std::list;

Scenerio_Tree_Packer::Scenerio_Tree_Packer(const list<Box> &boxes, const Container &container,
    std::vector<unsigned int> scenerios_in_level):A_Packer(convert_boxes_to_elements(boxes),container),scenerios_in_level(std::move(scenerios_in_level)) {
}

list<std::unique_ptr<A_Insertion_Coordinates>> Scenerio_Tree_Packer::pack() {
    list<std::unique_ptr<A_Insertion_Coordinates>> packing_coordinates;

    return packing_coordinates;
}
