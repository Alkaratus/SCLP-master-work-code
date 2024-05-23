//
// Created by kubam on 08.03.2024.
//

#ifndef MASTERS_WORK_PACKER_H
#define MASTERS_WORK_PACKER_H
#include <list>
#include <memory>
#include "Box.h"
#include "Container.h"
#include "Elements_Group.h"
#include "Simple_Block.h"
#include "Complex_Block.h"

class Packer {
    std::list<std::shared_ptr<Insertable_Element>> elements;
    Container container;
    unsigned int max_number_of_simple_blocks;
    unsigned int max_number_of_complex_block_merges;
    unsigned int min_fill_ratio;
    Insertable_Element *select_element(Free_Space &selected_free_space) const;
    void delete_elements_containing_element_with_id(unsigned int id);
    void delete_elements_with_id(unsigned int id);
public:
    Packer(const std::list<Box>& boxes, const Container& container);
    void create_elements_rotations();
    void pack();

};

std::list<Elements_Group>group_elements_in_list(const std::list<std::shared_ptr<Insertable_Element>> &elements);
std::list<Simple_Block>create_simple_blocks(const std::list<Elements_Group> &groups);
std::list<Complex_Block>create_complex_block(const std::list<Elements_Group> &groups);

#endif //MASTERS_WORK_PACKER_H
