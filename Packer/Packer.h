//
// Created by kubam on 08.03.2024.
//
#ifndef MASTERS_WORK_PACKER_H
#define MASTERS_WORK_PACKER_H

#include <list>
#include <memory>
#include <climits>
#include "A_Packer.h"
#include "Box.h"
#include "Visitor.h"
#include "Container.h"
#include "Elements_Group.h"
#include "Simple_Block.h"
#include "Complex_Block.h"

class Packer: public Visitor {
    std::list<std::shared_ptr<Insertable_Element>> elements;
    Container container;
    unsigned int max_number_of_simple_blocks=UINT_MAX;
    unsigned int max_number_of_complex_block_merges=UINT_MAX;
    unsigned int min_fill_ratio=MIN_FILL_RATIO;

    Insertable_Element *select_element(Free_Space &selected_free_space) const;

    void create_blocks();
    void delete_element(Insertable_Element *element);

    ///Creates rotations of elements in Y axis
    void create_elements_rotations();
public:
    Packer(const std::list<Box>& boxes, const Container& container);
    std::list<std::unique_ptr<A_Insertion_Coordinates>> pack();

    void visit(Box *box) override;
    void visit(Simple_Block *block) override;
    void visit(Complex_Block *block) override;
};

bool compare_elements_ptr_by_lengths(const std::shared_ptr<Insertable_Element>& first,const std::shared_ptr<Insertable_Element>& second);

/// Group elements in list by sizes
std::list<Elements_Group>group_elements_in_list(const std::list<std::shared_ptr<Insertable_Element>> &elements);
std::list<std::shared_ptr<Simple_Block>>create_simple_blocks(const std::list<Elements_Group> &groups);
std::list<std::shared_ptr<Complex_Block>> create_complex_block(std::list<Elements_Group> groups, Container &container);

std::vector<std::list<Elements_Group>::iterator>create_chain_of_iterators(std::list<Elements_Group> &groups, int elements_in_chain);
bool increment_chain_of_iterators(std::vector<std::list<Elements_Group>::iterator>&iterators,std::list<Elements_Group>::iterator end);

#endif //MASTERS_WORK_PACKER_H
