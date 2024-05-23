//
// Created by kubam on 07.03.2024.
//

#include "Elements_Group.h"

#include <utility>



Elements_Group::Elements_Group(const std::list<Insertable_Element*>& elements_pointers):
elements_pointers(elements_pointers.begin(),elements_pointers.end()){

}

const std::vector<Insertable_Element*> &Elements_Group::get_elements_pointers() const {
    return elements_pointers;
}

Element_3D Elements_Group::get_group_element_properties() const{
    return *elements_pointers[0];
}


