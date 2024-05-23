//
// Created by kubam on 01.03.2024.
//

#ifndef MASTERS_WORK_BOX_H
#define MASTERS_WORK_BOX_H


#include "Insertable_Element.h"

class Box: public Insertable_Element{
public:
    Box(unsigned int width,unsigned int depth,unsigned int height);
    Box(const Box& other);
    std::shared_ptr<Insertable_Element> get_rotated_element() override;
    bool contains_element_with_id(unsigned int id) override;
};


#endif //MASTERS_WORK_BOX_H
