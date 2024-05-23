//
// Created by kubam on 17.03.2024.
//

#include "Insertion_Coordinates.h"


Insertion_Coordinates::Insertion_Coordinates(Insertable_Element *inserted_element, Point_3D start_point):
inserted_element(inserted_element),start_point(start_point) {

}

const Insertable_Element *Insertion_Coordinates::get_inserted_element() const {
    return inserted_element;
}

Point_3D Insertion_Coordinates::get_start_point() const{
    return start_point;
}

Point_3D Insertion_Coordinates::get_wider_point() const {
    Point_3D wider_point(start_point);
    wider_point.set_x(start_point.get_x()+inserted_element->get_width());
    return wider_point;
}

Point_3D Insertion_Coordinates::get_deeper_point() const {
    Point_3D deeper_point(start_point);
    deeper_point.set_z(start_point.get_z()+inserted_element->get_depth());
    return deeper_point;
}

Point_3D Insertion_Coordinates::get_end_point() const {
    Point_3D end_point(start_point);
    end_point.set_x(start_point.get_x()+inserted_element->get_width());
    end_point.set_z(start_point.get_z()+inserted_element->get_depth());\
    return end_point;
}


