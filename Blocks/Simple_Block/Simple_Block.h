//
// Created by kubam on 01.03.2024.
//

#ifndef MASTERS_WORK_SIMPLE_BLOCK_H
#define MASTERS_WORK_SIMPLE_BLOCK_H

#include <vector>
#include "Block.h"
#include "Block_Elements_Numbers.h"

class Simple_Block: public Block{
    std::vector<std::vector<std::vector<Insertable_Element*>>>block_elements_pointers;
public:
    ///Creates simple block from 3D-set
    explicit Simple_Block(const std::vector<std::vector<std::vector<Insertable_Element*>>>&block_elements_pointers);
    Simple_Block(std::vector<Insertable_Element*> &block_elements_pointers, const Block_Elements_Numbers &elements_numbers);
    Simple_Block(const Simple_Block &block);
    void rotate_in_x();
    void rotate_in_y() override;
    void rotate_in_z();


    std::vector<std::vector<std::vector<Insertable_Element*>>>get_block_elements_pointers();
    std::unique_ptr<Insertable_Element> get_element_copy() override;
    std::unique_ptr<Insertable_Element> get_element_rotated_in_y() override;
    std::unique_ptr<Insertable_Element> get_element_rotated_in_x() const;
    std::unique_ptr<Insertable_Element> get_element_rotated_in_z() const;

    bool contains_element_with_id(unsigned int id) override;

    void accept(Visitor *packer) override;
};

Element_3D calculate_block_size(const Element_3D& single_element_dimensions, const Block_Elements_Numbers& elements_numbers);

#endif //MASTERS_WORK_SIMPLE_BLOCK_H
