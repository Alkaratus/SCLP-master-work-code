//
// Created by kubam on 02.03.2024.
//

#include "Free_Space.h"

unsigned int Free_Space::next_id=0;

Free_Space::Free_Space(const Point_3D &start, const unsigned int width, const unsigned int depth, const unsigned int height):
        Element_3D(width,depth,height), id(++next_id), start_point(start){

}

Point_3D Free_Space::get_start_corner() const {
    return start_point;
}

Point_3D Free_Space::get_wider_corner() const {
    return Point_3D(start_point.get_x() + get_width(), start_point.get_y(), start_point.get_z());
}

Point_3D Free_Space::get_deeper_corner() const {
    return Point_3D(start_point.get_x(), start_point.get_y(), start_point.get_z() + get_depth());
}

Point_3D Free_Space::get_end_corner() const {
    return Point_3D(start_point.get_x() + get_width(), start_point.get_y(), start_point.get_z() + get_depth());
}

bool Free_Space::can_element_be_inserted(const Insertable_Element *element) const {
    return get_width()>=element->get_width()&& get_depth()>=element->get_depth() && get_height()>=element->get_height();
}

bool Free_Space::is_point_deep_inside(const Point_2D &point) const{
    return start_point.get_x()<point.get_x() && start_point.get_x()+get_width()>point.get_x() &&
    start_point.get_z()<point.get_z() && start_point.get_z()+get_depth()>point.get_z();
}

bool Free_Space::is_point_inside(const Point_2D &point) const {
    return start_point.get_x()<=point.get_x() && start_point.get_x()+get_width()>=point.get_x() &&
    start_point.get_z()<=point.get_z() && start_point.get_z()+get_depth()>=point.get_z();
}

std::string Free_Space::get_properties() const{
    std::string properties= "ID: "+std::to_string(id)+" Start point: ("+std::to_string(start_point.get_x())+","+
            std::to_string(start_point.get_z())+","+std::to_string(start_point.get_y())+") "+ Element_3D::get_properties();
    return properties;
}

unsigned int Free_Space::get_id() const {
    return id;
}

bool Free_Space::is_related_with_another(const Free_Space &other) const {
    if(get_start_corner()==other.get_start_corner() || get_wider_corner()==other.get_wider_corner() || get_deeper_corner()==other.get_deeper_corner() ||
    get_end_corner()==other.get_end_corner()) {
        return true;
    }
    if(
    get_start_corner().get_z()<=other.get_start_corner().get_z()&& get_deeper_corner().get_z()>other.get_start_corner().get_z() &&
    other.get_start_corner().get_x()<=get_start_corner().get_x()&& other.get_wider_corner().get_x()>get_start_corner().get_x() ||
    get_start_corner().get_x()<=other.get_start_corner().get_x()&& get_wider_corner().get_x()>other.get_start_corner().get_x() &&
    other.get_start_corner().get_z()<=get_start_corner().get_z()&& other.get_deeper_corner().get_z()>get_start_corner().get_z()

    ) {
        return true;
    }
    return false;
}

bool Free_Space::is_inside_another(const Free_Space &other) const {
    bool start_inside=other.is_point_inside(get_start_corner());
    bool end_inside=other.is_point_inside(get_end_corner());
    return start_inside && end_inside;
}

bool Free_Space::is_adjacent_to_another_in_x(const Free_Space &other) const {
    return get_end_corner().get_x()==other.get_start_corner().get_x() && ((get_start_corner().get_z()<=other.get_start_corner().get_z() && get_end_corner().get_z()>other.get_start_corner().get_z()) ||
    (other.get_start_corner().get_z()<=get_start_corner().get_z() && other.get_end_corner().get_z()>get_start_corner().get_z()));
}

bool Free_Space::is_adjacent_to_another_in_z(const Free_Space &other) const {
    return get_end_corner().get_z()==other.get_start_corner().get_z() && ((get_start_corner().get_x()<=other.get_start_corner().get_x() && get_end_corner().get_x()>other.get_start_corner().get_x()) ||
    (other.get_start_corner().get_x()<=get_start_corner().get_x() && other.get_end_corner().get_x()>get_start_corner().get_x()));
}
