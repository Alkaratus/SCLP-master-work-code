//
// Created by kubam on 12.03.2024.
//

#include "Complex_Block.h"

Complex_Block::Complex_Block(const std::vector <Insertable_Element*>& block_elements, axis merge_axis):
Insertable_Element(calculate_block_size(block_elements,merge_axis).get_width(),
                   calculate_block_size(block_elements,merge_axis).get_depth(),
                   calculate_block_size(block_elements,merge_axis).get_height()), merge_axis(merge_axis) {
}

//TODO: do zrobienia
bool Complex_Block::contains_element_with_id(unsigned int id) {
    return false;
}

//TODO: trzeba zaimplementować wzorzec prototypu
std::shared_ptr<Insertable_Element> Complex_Block::get_rotated_element() {
    return nullptr;
}

Element_3D calculate_block_size(std::vector<Insertable_Element*> block_elements, axis merge_axis) {
    unsigned int sizes_sum_in_dimension=0;
    switch(merge_axis){
        case axis::X :{
            for(auto element:block_elements){
                sizes_sum_in_dimension+=element->get_width();
            }
            return {sizes_sum_in_dimension,block_elements[0]->get_depth(),block_elements[0]->get_height()};
        }
        case axis::Z:{
            for(auto element:block_elements){
                sizes_sum_in_dimension+=element->get_depth();
            }
            return {block_elements[0]->get_width(),sizes_sum_in_dimension,block_elements[0]->get_height()};
        }
        case axis::Y:{
            for(auto element:block_elements){
                sizes_sum_in_dimension+=element->get_height();
            }
            return {block_elements[0]->get_width(),block_elements[0]->get_depth(),sizes_sum_in_dimension};
        }
        default:{
            throw std::string("Invalid axis");
        }
    }
}
