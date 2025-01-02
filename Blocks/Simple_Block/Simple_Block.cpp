//
// Created by kubam on 01.03.2024.
//

#include "Simple_Block.h"
#include "../Visitor/Visitor.h"

using std::vector, std::shared_ptr;


Element_3D calculate_block_size(Insertable_Element* single_element_pointer,const Block_Elements_Numbers &elements_numbers);

Element_3D calculate_block_size(const vector<vector<vector<Insertable_Element*>>>& block_elements_pointers);

Simple_Block::Simple_Block(vector<Insertable_Element*> &block_elements_pointers, const Block_Elements_Numbers &elements_numbers):
Block(
        calculate_block_size(block_elements_pointers[0],elements_numbers).get_width(),
        calculate_block_size(block_elements_pointers[0],elements_numbers).get_depth(),
        calculate_block_size(block_elements_pointers[0],elements_numbers).get_height()
        ){
    vector<Insertable_Element*> row(elements_numbers.get_elements_number_in_width(), nullptr);
    vector<vector<Insertable_Element*>> layer(elements_numbers.get_elements_number_in_depth(),row);
    this->block_elements_pointers=vector<vector<vector<Insertable_Element*>>>(elements_numbers.get_elements_number_in_height(),layer);
    //TODO trzeba dodać weryfikację ilości elementów w vector
    unsigned int vector_index=0;
    for(unsigned int i=0;i<elements_numbers.get_elements_number_in_height();i++){
        for(unsigned int j=0;j<elements_numbers.get_elements_number_in_depth();j++){
            for(unsigned int k=0;k<elements_numbers.get_elements_number_in_width();k++){
                this->block_elements_pointers[i][j][k]=block_elements_pointers[vector_index];
                vector_index++;
            }
        }
    }
}

Simple_Block::Simple_Block(const Simple_Block &block):Block(block) {
    block_elements_pointers=block.block_elements_pointers;
}

Simple_Block::Simple_Block(const vector<vector<vector<Insertable_Element*>>>& block_elements_pointers):
        Block(
        calculate_block_size(block_elements_pointers).get_width(),
        calculate_block_size(block_elements_pointers).get_depth(),
        calculate_block_size(block_elements_pointers).get_height()
        ){
    vector<Insertable_Element*> row(block_elements_pointers[0][0].size(), nullptr);
    vector layer(block_elements_pointers[0].size(),row);
    this->block_elements_pointers=vector(block_elements_pointers.size(),layer);
    for(unsigned int i=0;i<block_elements_pointers.size();i++){
        for(unsigned int j=0;j<block_elements_pointers[0].size();j++){
            for(unsigned int k=0;k<block_elements_pointers[0][0].size();k++){
                this->block_elements_pointers[i][j][k]=block_elements_pointers[i][j][k];
            }
        }
    }
}

void Simple_Block::rotate_in_x() {
    set_height(get_height()^get_depth());
    set_depth(get_height()^get_depth());
    set_height(get_height()^get_depth());
    vector<Insertable_Element*> row(block_elements_pointers[0][0].size(), nullptr);
    vector layer(block_elements_pointers.size(),row);
    vector new_cube(block_elements_pointers[0].size(),layer);
    //TODO: trzeba wypełnić wektor3D
}

void Simple_Block::rotate_in_y() {
    set_width(get_width()^get_depth());
    set_depth(get_width()^get_depth());
    set_width(get_width()^get_depth());
    vector<Insertable_Element*> row(block_elements_pointers[0].size(), nullptr);
    vector layer(block_elements_pointers[0][0].size(),row);
    vector new_cube(block_elements_pointers.size(),layer);
    //TODO: trzeba wypełnić wektor3D
}

void Simple_Block::rotate_in_z() {
    set_width(get_width()^get_height());
    set_height(get_width()^get_height());
    set_width(get_width()^get_height());
    vector<Insertable_Element*> row(block_elements_pointers.size(), nullptr);
    vector layer(block_elements_pointers[0].size(),row);
    vector new_cube(block_elements_pointers[0][0].size(),layer);
    //TODO: trzeba wypełnić wektor3D
}

bool Simple_Block::contains_element_with_id(const unsigned int id) {
    for (const auto& layer:block_elements_pointers){
        for(const auto& row:layer){
            for(const auto& element:row){
                if(element->get_id()==id ||element->contains_element_with_id(id)){
                    return true;
                }
            }
        }
    }
    return false;
}

shared_ptr<Insertable_Element> Simple_Block::get_element_rotated_in_y() {
    std::shared_ptr<Simple_Block>rotated(new Simple_Block(*this));
    rotated->rotate_in_y();
    return rotated;
}

std::shared_ptr<Insertable_Element> Simple_Block::get_element_rotated_in_x() {
    std::shared_ptr<Simple_Block>rotated(new Simple_Block(*this));
    rotated->rotate_in_x();
    return rotated;
}


void Simple_Block::accept(Visitor *visitor) {
    visitor->visit(this);
}

vector<vector<vector<Insertable_Element*>>> Simple_Block::get_block_elements_pointers() {
    return block_elements_pointers;
}

Element_3D calculate_block_size(const Element_3D &single_element,
                                const Block_Elements_Numbers &elements_numbers) {
    return {single_element.get_width()*elements_numbers.get_elements_number_in_width(),
            single_element.get_depth()*elements_numbers.get_elements_number_in_depth(),
            single_element.get_height()*elements_numbers.get_elements_number_in_height()};
}

Element_3D calculate_block_size(Insertable_Element *single_element_pointer, const Block_Elements_Numbers &elements_numbers) {
    return {single_element_pointer->get_width()*elements_numbers.get_elements_number_in_width(),
                      single_element_pointer->get_depth()*elements_numbers.get_elements_number_in_depth(),
                      single_element_pointer->get_height()*elements_numbers.get_elements_number_in_height()};
}

Element_3D calculate_block_size(const vector<vector<vector<Insertable_Element*>>>& block_elements_pointers){
    return Element_3D(block_elements_pointers[0][0][0]->get_width()*block_elements_pointers[0][0].size(),
                      block_elements_pointers[0][0][0]->get_depth()*block_elements_pointers[0].size(),
                      block_elements_pointers[0][0][0]->get_height()*block_elements_pointers.size());
}