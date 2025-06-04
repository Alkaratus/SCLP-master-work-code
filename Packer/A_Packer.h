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
#include "Elements_Group.h"

constexpr unsigned int FILL_SCALE=1000;
constexpr unsigned int MIN_FILL_RATIO=100;

class A_Packer: public Visitor {
    std::list<std::unique_ptr<Insertable_Element>> elements;
    Container container;
    std::list<Elements_Group> elements_groups;

    unsigned int max_number_of_simple_blocks=UINT_MAX;
    unsigned int max_number_of_complex_block_merges=UINT_MAX;
    unsigned int min_fill_ratio=MIN_FILL_RATIO;
    ///Creates rotations of elements in Y axis
    void create_elements_rotations();

protected:
    A_Packer(const std::list<std::unique_ptr<Insertable_Element>>& elements,const Container& container);
    A_Packer(const A_Packer& other);
    A_Packer(const A_Packer* other);
    std::list<std::unique_ptr<Insertable_Element>>& get_elements();
    Container& get_container();

    void set_elements_groups(const std::list<Elements_Group>& _elements_groups);
public:
    virtual std::list<std::unique_ptr<A_Insertion_Coordinates>> pack()=0;
    [[nodiscard]] const Container& get_container_data() const;
    [[nodiscard]] const std::list<Elements_Group>& get_elements_groups_data() const;

};

std::list<std::unique_ptr<Insertable_Element>> convert_boxes_to_elements(const std::list<Box>& boxes);

/// Group elements in list by sizes
std::list<Elements_Group>group_elements_in_list(const std::list<std::unique_ptr<Insertable_Element>> &elements);
std::list<std::unique_ptr<Simple_Block>>create_simple_blocks(const std::list<Elements_Group> &groups);
std::list<std::unique_ptr<Complex_Block>> create_complex_block(std::list<Elements_Group> groups, Container &container);

std::vector<std::list<Elements_Group>::iterator>create_chain_of_iterators(std::list<Elements_Group> &groups, int elements_in_chain);
bool increment_chain_of_iterators(std::vector<std::list<Elements_Group>::iterator>&iterators,std::list<Elements_Group>::iterator end);

bool compare_elements_ptr_by_lengths(const std::unique_ptr<Insertable_Element>& first,const std::unique_ptr<Insertable_Element>& second);


#endif //A_PACKER_H
