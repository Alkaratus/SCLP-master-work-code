//
// Created by kubam on 18.09.2024.
//

#include "Block_Insertion_Coordinates.h"

Block_Insertion_Coordinates::Block_Insertion_Coordinates(Point_3D startPoint, Block *block, std::vector<std::unique_ptr<A_Insertion_Coordinates>> sub_elements): A_Insertion_Coordinates(
        startPoint, Element_3D(block->get_width(), block->get_depth(), block->get_height())),
    element_id(block->get_id()),sub_elements(std::move(sub_elements)) {
}

std::string Block_Insertion_Coordinates::accept(Insertion_Coordinates_Displayer* displayer) {
    return displayer->display_block_coordinates(this);
}

unsigned int Block_Insertion_Coordinates::get_element_id() const {
    return element_id;
}

std::vector<std::unique_ptr<A_Insertion_Coordinates>>& Block_Insertion_Coordinates::get_sub_elements() {
    return sub_elements;
}

