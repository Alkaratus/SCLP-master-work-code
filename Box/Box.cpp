//
// Created by kubam on 01.03.2024.
//

#include "Box.h"
#include "../Visitor/Visitor.h"

Box::Box(unsigned int width, unsigned int depth, unsigned int height) : Insertable_Element(width, depth, height){

}

void Box::rotate_in_y() {
    set_width(get_width()^get_depth());
    set_depth(get_width()^get_depth());
    set_width(get_width()^get_depth());
}

std::shared_ptr<Insertable_Element> Box::get_element_rotated_in_y() {
    std::shared_ptr<Box>rotated(new Box(*this));
    rotated->rotate_in_y();
    return rotated;
}

Box::Box(const Box &other): Insertable_Element(other) {
}

bool Box::contains_element_with_id(unsigned int id) {
    return false;
}

void Box::accept(Visitor *visitor) {
    visitor->visit(this);
}

