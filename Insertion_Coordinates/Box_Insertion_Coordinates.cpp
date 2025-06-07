//
// Created by kubam on 18.09.2024.
//

#include "Box_Insertion_Coordinates.h"

Box_Insertion_Coordinates::Box_Insertion_Coordinates(Point_3D start, Box *box):A_Insertion_Coordinates(start,Element_3D(box->get_width(),box->get_depth(),box->get_height())),element_id(box->get_id()) {
}

std::string Box_Insertion_Coordinates::accept(I_Coordinates_Displayer* displayer) {
    return displayer->display_box_coordinates(this);
}

unsigned int Box_Insertion_Coordinates::get_element_id() const{
    return element_id;
}

std::unique_ptr<A_Insertion_Coordinates> Box_Insertion_Coordinates::create_copy() const {
    return std::make_unique<Box_Insertion_Coordinates>(*this);
}
