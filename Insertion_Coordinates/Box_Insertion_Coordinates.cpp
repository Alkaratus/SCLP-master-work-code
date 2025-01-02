//
// Created by kubam on 18.09.2024.
//

#include "Box_Insertion_Coordinates.h"

Box_Insertion_Coordinates::Box_Insertion_Coordinates(Point_3D start, Box *box):A_Insertion_Coordinates(start,Element_3D(box->get_width(),box->get_depth(),box->get_height())),element_id(box->get_id()) {
}

std::string Box_Insertion_Coordinates::accept(Insertion_Coordinates_Displayer* displayer) {
    return displayer->display_box_coordinates(this);
}

unsigned int Box_Insertion_Coordinates::get_element_id() {
    return element_id;
}
