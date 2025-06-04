//
// Created by kubam on 08.03.2024.
//
#ifndef MASTERS_WORK_PACKER_H
#define MASTERS_WORK_PACKER_H

#include <functional>
#include <list>
#include <map>
#include <memory>
#include "A_Packer.h"
#include "Box.h"
#include "Visitor.h"
#include "Container.h"


class Parallel_Tree_Packer;

class Packer: public A_Packer {
    unsigned int min_fill_ratio=80;

    Insertable_Element *select_element_by_volume(const Free_Space &selected_free_space);
    Insertable_Element *select_element_by_surface(const Free_Space &selected_free_space);
    Insertable_Element *select_element_by_first_satisfying_element(const Free_Space &selected_free_space);
    void create_blocks();
    void delete_element(Insertable_Element *element);

    std::function<Insertable_Element*(Packer*,Free_Space &selected_free_space)>select_method;


public:
    enum Element_Selecting_Method {
        by_max_volume,
        by_max_surface,
        by_first_satisfying_element
    };

    static const std::map<Element_Selecting_Method, std::function<Insertable_Element *(Packer *, Free_Space&)>> selection_methods;

    Packer(const std::list<Box>& boxes, const Container& container, Element_Selecting_Method method=by_max_volume);
    explicit Packer(const Parallel_Tree_Packer* other);
    std::list<std::unique_ptr<A_Insertion_Coordinates>> pack() override;
    std::unique_ptr<A_Insertion_Coordinates> pack_single_element(Insertable_Element *element);

    Insertable_Element *get_element_by_id(unsigned int id);

    void visit(Box *box) override;
    void visit(Simple_Block *block) override;
    void visit(Complex_Block *block) override;

    friend class Test;
};

#endif //MASTERS_WORK_PACKER_H
