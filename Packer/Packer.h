//
// Created by kubam on 08.03.2024.
//
#ifndef MASTERS_WORK_PACKER_H
#define MASTERS_WORK_PACKER_H

#include <list>
#include <memory>
#include "A_Packer.h"
#include "Box.h"
#include "Visitor.h"
#include "Container.h"


class Packer: public A_Packer {
    unsigned int min_fill_ratio=MIN_FILL_RATIO;

    Insertable_Element *select_element(Free_Space &selected_free_space);
    void create_blocks();
    void delete_element(Insertable_Element *element);


public:
    Packer(const std::list<Box>& boxes, const Container& container);
    std::list<std::unique_ptr<A_Insertion_Coordinates>> pack();

    void visit(Box *box) override;
    void visit(Simple_Block *block) override;
    void visit(Complex_Block *block) override;
};

#endif //MASTERS_WORK_PACKER_H
