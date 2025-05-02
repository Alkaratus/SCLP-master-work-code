//
// Created by kubam on 23.05.2024.
//

#include <algorithm>
#include "Container.h"
#include "Insertion_Coordinates_Factory.h"

using std::list, std::shared_ptr, std::make_shared;

std::array<unsigned int,3> calculate_anchor_distance(const Point_3D &first,const Point_3D &second);


shared_ptr<Container::Free_Space> Container::Anchored_Free_Space::create_free_space_above_inserted_element(
        const A_Insertion_Coordinates *inserted_element_coordinates) {
    auto insertion_start_point = inserted_element_coordinates->get_start_point();
    auto inserted_element = inserted_element_coordinates->get_sizes();
    return make_shared<Anchored_Free_Space>(Point_3D(insertion_start_point.get_x(),insertion_start_point.get_y()+inserted_element.get_height(),insertion_start_point.get_z()),
                                                       inserted_element.get_width(),inserted_element.get_depth(),
                                                       get_height()-inserted_element.get_height(),get_owner());
}

list<shared_ptr<Container::Free_Space>> Container::Anchored_Free_Space::add_free_spaces_on_sides_of_inserted_element(A_Insertion_Coordinates *inserted_element_coordinates) {
    list<shared_ptr<Container::Free_Space>> created_free_spaces;
    auto insertion_start_point = inserted_element_coordinates->get_start_point();
    auto inserted_element = inserted_element_coordinates->get_sizes();
    auto free_space_anchor_type=anchor_place;
    if(free_space_anchor_type==start || free_space_anchor_type==wider){
        created_free_spaces.emplace_back(get_slice_on_back_from(insertion_start_point.get_z()+inserted_element.get_depth()));
    }
    else{
        created_free_spaces.emplace_back(get_slice_on_front_from(insertion_start_point.get_z()));
    }
    if(free_space_anchor_type==start || free_space_anchor_type==deeper){
        created_free_spaces.emplace_back(get_slice_on_right_from(insertion_start_point.get_x()+inserted_element.get_width()));
    }
    else{
        created_free_spaces.emplace_back(get_slice_on_left_from(insertion_start_point.get_x()));
    }
    //TODO trzeba dodać relację między wolnymi przestrzeniami

    return created_free_spaces;
}

list<shared_ptr<Container::Free_Space>> Container::Anchored_Free_Space::create_free_space_from_related_free_spaces(A_Insertion_Coordinates *inserted_element_coordinates) {
    list<shared_ptr<Container::Free_Space>> new_free_spaces;
    auto insertion_start_point=inserted_element_coordinates->get_start_point();
    auto insertion_wider_point= inserted_element_coordinates->get_wider_point();
    auto insertion_deeper_point=inserted_element_coordinates->get_deeper_point();
    auto insertion_end_point=inserted_element_coordinates->get_end_point();

    auto related_free_spaces=get_related_free_spaces();
    for(auto &related_free_space: related_free_spaces){
        bool related_free_space_affected=false;
        if(related_free_space->is_point_deep_inside(insertion_start_point)){
            get_slice_on_left_from(insertion_start_point.get_x());
            get_slice_on_front_from(insertion_start_point.get_z());
            related_free_space_affected=true;
        }
        if(related_free_space->is_point_deep_inside(insertion_wider_point)){
            get_slice_on_right_from(insertion_wider_point.get_x());
            get_slice_on_front_from(insertion_wider_point.get_z());
            related_free_space_affected=true;
        }
        if(related_free_space->is_point_deep_inside(insertion_deeper_point)){
            get_slice_on_left_from(insertion_deeper_point.get_x());
            get_slice_on_back_from(insertion_deeper_point.get_z());
            related_free_space_affected=true;
        }
        if(related_free_space->is_point_deep_inside(insertion_end_point)){
            get_slice_on_right_from(insertion_end_point.get_x());
            get_slice_on_back_from(insertion_end_point.get_z());
            related_free_space_affected=true;
        }

        //TODO tę funkcję trzeba zrobić inaczej
        if(related_free_space_affected){
            //get_owner()->free_spaces.erase()
            //erase(related_free_space);
        }
    }

    return new_free_spaces;
}

Container::Anchored_Free_Space::Anchored_Free_Space(Point_3D start, unsigned int width, unsigned int depth,unsigned int height,
                                                    Container *owner):
        Free_Space(start, width, depth, height,owner),
        anchor_corner(get_start_corner()),
        anchor_lengths(calculate_anchor_distance(Point_3D(), anchor_corner)),
        anchor_place(anchor_place_type::start){
    auto next_corner=get_wider_corner();
    auto next_anchor= calculate_anchor_distance(Point_3D(owner->get_width(),0,0),next_corner);
    if(is_first_anchor_smaller_than_second(next_anchor,anchor_lengths)){
        anchor_corner=next_corner;
        anchor_lengths=next_anchor;
        anchor_place=anchor_place_type::wider;
    }
    next_corner=get_deeper_corner();
    next_anchor= calculate_anchor_distance(Point_3D(0,0,owner->get_depth()),next_corner);
    if(is_first_anchor_smaller_than_second(next_anchor,anchor_lengths)){
        anchor_corner=next_corner;
        anchor_lengths=next_anchor;
        anchor_place=anchor_place_type::deeper;
    }
    next_corner=get_end_corner();
    next_anchor= calculate_anchor_distance(Point_3D(owner->get_width(),0,owner->get_depth()),next_corner);
    if(is_first_anchor_smaller_than_second(next_anchor,anchor_lengths)){
        anchor_corner=next_corner;
        anchor_lengths=next_anchor;
        anchor_place=anchor_place_type::end;
    }
}

Container::anchor_place_type Container::Anchored_Free_Space::get_anchor_corner_type() {
    return anchor_place;
}

Point_3D Container::Free_Space::get_anchor_corner() {
    return get_start_corner();
}

Point_3D Container::Anchored_Free_Space::get_anchor_corner() {
    return anchor_corner;
}

std::unique_ptr<A_Insertion_Coordinates> Container::Anchored_Free_Space::get_insertion_coordinates_for_element(Insertable_Element *element) {
    auto anchor=get_anchor_corner();
    Insertion_Coordinates_Factory factory;
    switch(anchor_place){
        case start:{
            return factory.create_insertion_coordinate(element,anchor);
        }
        case wider: {
            return factory.create_insertion_coordinate(element,Point_3D(anchor.get_x()-element->get_width(),
                                                                        anchor.get_y(),anchor.get_z()));
        }
        case deeper:{
            return factory.create_insertion_coordinate(element,Point_3D(anchor.get_x(),anchor.get_y(),
                                                                        anchor.get_z()-element->get_depth()));
        }
        case end:{
            return factory.create_insertion_coordinate(element,Point_3D(anchor.get_x()-element->get_width(),anchor.get_y(),
                                                                        anchor.get_z()-element->get_depth()));
        }
    }
}

shared_ptr<Container::Free_Space> Container::Anchored_Free_Space::get_slice_on_left_from(unsigned int point_x_value) {
    return std::make_shared<Anchored_Free_Space>(get_start_corner(), point_x_value - get_start_corner().get_x(), get_depth(),
                                                            get_height(), get_owner());
}

shared_ptr<Container::Free_Space> Container::Anchored_Free_Space::get_slice_on_right_from(unsigned int point_x_value) {
    auto new_free_space_start=get_start_corner();
    new_free_space_start.set_x(new_free_space_start.get_x() + point_x_value);
    return std::make_shared<Anchored_Free_Space>(new_free_space_start, get_start_corner().get_x() + get_width() - point_x_value,
                                                            get_depth(),get_height(), get_owner());
}

shared_ptr<Container::Free_Space> Container::Anchored_Free_Space::get_slice_on_front_from(unsigned int point_z_value) {
    return std::make_shared<Anchored_Free_Space>(get_start_corner(), get_width(), point_z_value- get_start_corner().get_z(),
                                                            get_height(), get_owner());
}

shared_ptr<Container::Free_Space> Container::Anchored_Free_Space::get_slice_on_back_from(unsigned int point_z_value) {
    auto new_free_space_start=get_start_corner();
    new_free_space_start.set_z(new_free_space_start.get_z()+point_z_value);
    return std::make_unique<Anchored_Free_Space>(new_free_space_start, get_width(), get_start_corner().get_z()+get_depth() - point_z_value,
                                                            get_height(), get_owner());
}

std::array<unsigned int, 3> Container::Anchored_Free_Space::get_anchor_lengths() const {
    return anchor_lengths;
}


std::array<unsigned int,3> calculate_anchor_distance(const Point_3D &first,const Point_3D &second){
    std::array<unsigned int,3>lengths={
            first.get_x()<second.get_x()?second.get_x()-first.get_x():first.get_x()-second.get_x(),
            first.get_y()<second.get_y()?second.get_y()-first.get_y():first.get_y()-second.get_y(),
            first.get_z()<second.get_z()?second.get_z()-first.get_z():first.get_z()-second.get_z()};
    std::sort(lengths.begin(),lengths.end());
    return lengths;
}