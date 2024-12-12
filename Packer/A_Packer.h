//
// Created by kubam on 03.10.2024.
//

#ifndef A_PACKER_H
#define A_PACKER_H

#include <list>
#include <memory>
#include <climits>
#include "Box.h"
#include "Visitor.h"
#include "Container.h"
#include "Elements_Group.h"
#include "Simple_Block.h"
#include "Complex_Block.h"

constexpr unsigned int MIN_FILL_RATIO=100;

class A_Packer: public Visitor {
    std::list<std::shared_ptr<Insertable_Element>> elements;
    Container container;
    unsigned int max_number_of_simple_blocks=UINT_MAX;
    unsigned int max_number_of_complex_block_merges=UINT_MAX;
    unsigned int min_fill_ratio=MIN_FILL_RATIO;

    virtual std::list<std::unique_ptr<A_Insertion_Coordinates>> pack();
};



#endif //A_PACKER_H
