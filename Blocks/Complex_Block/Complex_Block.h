//
// Created by kubam on 12.03.2024.
//

#ifndef MASTERS_WORK_COMPLEX_BLOCK_H
#define MASTERS_WORK_COMPLEX_BLOCK_H

#include <vector>
#include "Block.h"

enum axis{
    X,//Axis of width
    Y,//Axis of height
    Z //Axis of depth
};

class Complex_Block: public Block {
    std::vector<Insertable_Element*>block_elements_IDs;
    axis merge_axis;
public:
    Complex_Block(const std::vector<Insertable_Element*>&block_elements,axis merge_axis);
    std::shared_ptr<Insertable_Element> get_element_rotated_in_y() override;
    bool contains_element_with_id(unsigned int id) override;

};

Element_3D calculate_block_size(std::vector<Insertable_Element*>block_elements_IDs,axis merge_axis);

#endif //MASTERS_WORK_COMPLEX_BLOCK_H
