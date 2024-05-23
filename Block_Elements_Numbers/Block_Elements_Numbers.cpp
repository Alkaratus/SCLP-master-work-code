//
// Created by kubam on 11.03.2024.
//

#include "Block_Elements_Numbers.h"

Block_Elements_Numbers::Block_Elements_Numbers(unsigned int elements_in_width, unsigned int elements_in_depth,
                                               unsigned int elements_in_height):
        elements_number_in_width(elements_in_width),
        elements_number_in_depth(elements_in_depth),
        elements_number_in_height(elements_in_height){
}

unsigned int Block_Elements_Numbers::get_elements_number_in_block() const{
    return elements_number_in_width * elements_number_in_depth * elements_number_in_height;
}

unsigned int Block_Elements_Numbers::get_elements_number_in_width() const{
    return elements_number_in_width;
}

unsigned int Block_Elements_Numbers::get_elements_number_in_depth() const{
    return elements_number_in_depth;
}

unsigned int Block_Elements_Numbers::get_elements_number_in_height() const{
    return elements_number_in_height;
}

void Block_Elements_Numbers::set_elements_number_in_width(unsigned int number) {
    elements_number_in_width=number;
}

void Block_Elements_Numbers::set_elements_number_in_depth(unsigned int number) {
    elements_number_in_depth=number;
}

void Block_Elements_Numbers::set_elements_number_in_height(unsigned int number) {
    elements_number_in_height=number;
}

