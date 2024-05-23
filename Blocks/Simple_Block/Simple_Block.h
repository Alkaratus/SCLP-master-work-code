//
// Created by kubam on 01.03.2024.
//

#ifndef MASTERS_WORK_SIMPLE_BLOCK_H
#define MASTERS_WORK_SIMPLE_BLOCK_H

#include <vector>
#include "Insertable_Element.h"
#include "Box.h"
#include "Block_Elements_Numbers.h"

class Simple_Block: public Insertable_Element{
    std::vector<std::vector<std::vector<std::shared_ptr<Insertable_Element>>>>block_elements_pointers;
public:
    explicit Simple_Block(const std::vector<std::vector<std::vector<std::shared_ptr<Insertable_Element>>>>&block_elements_pointers);
    Simple_Block(std::vector<std::shared_ptr<Insertable_Element>> block_elements_pointers, const Block_Elements_Numbers &elements_numbers);
    void rotate_in_x();
    void rotate_in_z();
    std::shared_ptr<Insertable_Element> get_rotated_element() override;
    bool contains_element_with_id(unsigned int id) override;
};

Element_3D calculate_block_size(const Element_3D& single_element_dimensions, const Block_Elements_Numbers& elements_numbers);

#endif //MASTERS_WORK_SIMPLE_BLOCK_H
