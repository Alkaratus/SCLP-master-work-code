//
// Created by kubam on 03.10.2024.
//

#ifndef A_PACKER_H
#define A_PACKER_H

#include <list>
#include <memory>
#include <climits>
#include "Visitor.h"
#include "Container.h"

constexpr unsigned int MIN_FILL_RATIO=100;

class A_Packer: public Visitor {
    std::list<std::shared_ptr<Insertable_Element>> elements;
    Container container;
    unsigned int max_number_of_simple_blocks=UINT_MAX;
    unsigned int max_number_of_complex_block_merges=UINT_MAX;
    unsigned int min_fill_ratio=MIN_FILL_RATIO;
    ///Creates rotations of elements in Y axis
    void create_elements_rotations();

protected:
    A_Packer(std::list<std::shared_ptr<Insertable_Element>> elements,const Container& container);
    std::list<std::shared_ptr<Insertable_Element>>& get_elements();
    Container& get_container();
public:
    virtual std::list<std::unique_ptr<A_Insertion_Coordinates>> pack()=0;

};

std::list<std::shared_ptr<Insertable_Element>> convert_boxes_to_elements(const std::list<Box>& boxes);

#endif //A_PACKER_H
