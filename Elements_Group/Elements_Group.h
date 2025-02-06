//
// Created by kubam on 07.03.2024.
//

#ifndef MASTERS_WORK_ELEMENTS_GROUP_H
#define MASTERS_WORK_ELEMENTS_GROUP_H

#include <vector>
#include <list>
#include "Insertable_Element.h"

class Elements_Group {
    std::vector<Insertable_Element*>elements_pointers;
    Element_3D group_element_properties;
public:
    explicit Elements_Group(const std::list<Insertable_Element *>& elements_pointers);
    const std::vector<Insertable_Element*>& get_elements_pointers() const;
    Element_3D get_group_element_properties() const;
};

#endif //MASTERS_WORK_ELEMENTS_GROUP_H
