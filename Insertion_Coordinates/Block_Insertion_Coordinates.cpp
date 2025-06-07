//
// Created by kubam on 18.09.2024.
//

#include "Block_Insertion_Coordinates.h"

Block_Insertion_Coordinates::Block_Insertion_Coordinates(Point_3D startPoint, Block *block, std::vector<std::unique_ptr<A_Insertion_Coordinates>> sub_elements): A_Insertion_Coordinates(
        startPoint, Element_3D(block->get_width(), block->get_depth(), block->get_height())),
    element_id(block->get_id()),sub_elements(std::move(sub_elements)) {
}

Block_Insertion_Coordinates::Block_Insertion_Coordinates(const Block_Insertion_Coordinates &other):A_Insertion_Coordinates(other.get_start_point(),get_sizes()) {
    std::vector<std::unique_ptr<A_Insertion_Coordinates>> new_sub_elements(sub_elements.size());
    for (unsigned int i = 0; i < sub_elements.size(); i++) {
        new_sub_elements[i] = sub_elements[i]->create_copy();
    }
    sub_elements = std::move(new_sub_elements);
}

std::string Block_Insertion_Coordinates::accept(I_Coordinates_Displayer* displayer) {
    return displayer->display_block_coordinates(this);
}

unsigned int Block_Insertion_Coordinates::get_element_id() const {
    return element_id;
}

std::vector<std::unique_ptr<A_Insertion_Coordinates>>& Block_Insertion_Coordinates::get_sub_elements() {
    return sub_elements;
}

std::unique_ptr<A_Insertion_Coordinates> Block_Insertion_Coordinates::create_copy() const {
    return std::make_unique<Block_Insertion_Coordinates>(*this);
}

