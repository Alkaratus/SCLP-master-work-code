//
// Created by kubam on 18.09.2024.
//

#include "Block_Insertion_Coordinates.h"

Block_Insertion_Coordinates::Block_Insertion_Coordinates(Point_3D startPoint, Block *block, std::vector<std::unique_ptr<A_Insertion_Coordinates>> sub_elements): A_Insertion_Coordinates(
        startPoint, Element_3D(block->get_width(), block->get_depth(), block->get_height())),
    element_id(block->get_id()),sub_elements(std::move(sub_elements)) {
}

std::string Block_Insertion_Coordinates::create_json() {
    std::string json="Block:{\n";
    json+="id:"+std::to_string(element_id)+",\n";
    for(auto &sub_element : sub_elements) {
        json+=sub_element->create_json()+'\n';
    }
    return json+"}";
}
