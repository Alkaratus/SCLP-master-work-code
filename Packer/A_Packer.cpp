//
// Created by kubam on 03.10.2024.
//

#include <utility>
#include "A_Packer.h"
#include "Box.h"

using std::list, std::shared_ptr;

A_Packer::A_Packer(std::list<std::shared_ptr<Insertable_Element>> elements, const Container& container):
elements(std::move(elements)),container(container) {
    create_elements_rotations();
}


void A_Packer::create_elements_rotations() {
    list<shared_ptr<Insertable_Element>>rotations;
    for(auto const &element:elements){
        auto rotated= element->get_element_rotated_in_y();
        if((*rotated)!=(*element) && (!container.cant_element_be_inserted(rotated.get()))){
            rotations.emplace_back(std::move(rotated));
        }
    }
    elements.merge(rotations);
}

list<shared_ptr<Insertable_Element>> & A_Packer::get_elements() {
    return elements;
}

Container& A_Packer::get_container() {
    return container;
}

void A_Packer::set_elements_groups(const std::list<Elements_Group> &_elements_groups) {
    elements_groups = _elements_groups;
}

const Container & A_Packer::get_container_data() const {
    return container;
}

const std::list<Elements_Group> & A_Packer::get_elements_groups_data() const {
    return elements_groups;
}

list<shared_ptr<Insertable_Element>> convert_boxes_to_elements(const list<Box>& boxes) {
    list<shared_ptr<Insertable_Element>> elements;
    for(auto &box:boxes){
        elements.emplace_back(std::make_shared<Box>(box));
    }
    return elements;
}
