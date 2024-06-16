//
// Created by kubam on 01.03.2024.
//

#include "Box.h"
#include "../Visitor/Visitor.h"

Box::Box(unsigned int width, unsigned int depth, unsigned int height) : Insertable_Element(width, depth, height){

}

std::shared_ptr<Insertable_Element> Box::get_rotated_element() {
    std::shared_ptr<Box>rotated(new Box(*this));
    rotated->rotate();
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

