//
// Created by kubam on 17.03.2024.
//

#include "A_Insertion_Coordinates.h"

#include <utility>

A_Insertion_Coordinates::A_Insertion_Coordinates(Point_3D start_point,Element_3D sizes): start_point(start_point),
                                                                                         sizes(std::move(sizes)) {
}

Point_3D A_Insertion_Coordinates::get_start_point() const{
    return start_point;
}

Point_3D A_Insertion_Coordinates::get_wider_point() const {
    Point_3D wider_point(start_point);
    wider_point.set_x(start_point.get_x()+sizes.get_width());
    return wider_point;
}

Point_3D A_Insertion_Coordinates::get_deeper_point() const {
    Point_3D deeper_point(start_point);
    deeper_point.set_z(start_point.get_z()+sizes.get_depth());
    return deeper_point;
}

Point_3D A_Insertion_Coordinates::get_end_point() const {
    Point_3D end_point(start_point);
    end_point.set_x(start_point.get_x()+sizes.get_width());
    end_point.set_z(start_point.get_z()+sizes.get_depth());\
    return end_point;
}

Element_3D A_Insertion_Coordinates::get_sizes() const{
    return sizes;
}

