//
// Created by kubam on 23.05.2024.
//

#include "Container.h"
#include "Insertion_Coordinates_Factory.h"
#include <algorithm>
#include <iostream>

using std::list, std::shared_ptr, std::make_shared;

Container::Free_Space::Free_Space(Point_3D start, unsigned int width, unsigned int depth, unsigned int height,
                                  Container *owner): External_Free_Space(start,width,depth,height),owner(owner) {
}

Container *Container::Free_Space::get_owner() {
    return owner;
}

std::list<std::shared_ptr<Container::Free_Space>> Container::Free_Space::get_related_free_spaces() {
    return related_free_spaces;
}

void Container::Free_Space::add_related_free_space(const std::shared_ptr<Free_Space>& free_space) {
    related_free_spaces.emplace_back(free_space);
}

void Container::Free_Space::remove_related_free_space(const shared_ptr<Free_Space> &free_space) {
    auto place=std::find(related_free_spaces.begin(),related_free_spaces.end(),free_space);
    if(place!=related_free_spaces.end()){
        related_free_spaces.erase(place);
    }
}

std::unique_ptr<A_Insertion_Coordinates> Container::Free_Space::get_insertion_coordinates_for_element(Insertable_Element *element) {
    Insertion_Coordinates_Factory factory;
    return factory.create_insertion_coordinate(element,get_start_corner());
}

shared_ptr<Container::Free_Space> Container::Free_Space::get_slice_on_left_from(unsigned int point_x_value) {
    auto slice_width=point_x_value - get_start_corner().get_x();
    if(slice_width==0){
        return nullptr;
    }
    return make_shared<Container::Free_Space>(get_start_corner(), slice_width, get_depth(),
                                                   get_height(), owner);
}

shared_ptr<Container::Free_Space> Container::Free_Space::get_slice_on_right_from(unsigned int point_x_value) {
    auto slice_width=get_start_corner().get_x() + get_width() - point_x_value;
    if(slice_width==0){
        return nullptr;
    }
    auto new_free_space_start=get_start_corner();
    new_free_space_start.set_x(point_x_value);
    return make_shared<Container::Free_Space>(new_free_space_start, slice_width,get_depth(),
                                                   get_height(), owner);
}

shared_ptr<Container::Free_Space> Container::Free_Space::get_slice_on_front_from(unsigned int point_z_value) {
    auto slice_depth=point_z_value- get_start_corner().get_z();
    if(slice_depth==0){
        return nullptr;
    }
    return make_shared<Container::Free_Space>(get_start_corner(), get_width(), slice_depth,
                                                   get_height(), owner);
}

shared_ptr<Container::Free_Space> Container::Free_Space::get_slice_on_back_from(unsigned int point_z_value) {
    auto slice_depth=get_start_corner().get_z()+get_depth() - point_z_value;
    if(slice_depth==0){
        return nullptr;
    }
    auto new_free_space_start=get_start_corner();
    new_free_space_start.set_z(point_z_value);
    return make_shared<Container::Free_Space>(new_free_space_start, get_width(), slice_depth,
                                                   get_height(), owner);
}

std::array<unsigned int, 3> Container::Free_Space::get_anchor_lengths() const{
    auto start_point=get_start_corner();
    std::array<unsigned int, 3>lengths={start_point.get_x(),start_point.get_z(),start_point.get_y()};
    std::sort(lengths.begin(),lengths.end());
    std::reverse(lengths.begin(),lengths.end());
    return lengths;
}

bool Container::Free_Space::do_point_affects(const Point_2D &point) {
    auto start_point=get_start_corner();
    return start_point.get_x()<point.get_x() && start_point.get_x()+get_width()>=point.get_x() &&
           start_point.get_z()<point.get_z() && start_point.get_z()+get_depth()>=point.get_z();
}

shared_ptr<Container::Free_Space> Container::Free_Space::create_free_space_above_inserted_element(const A_Insertion_Coordinates *inserted_element_coordinates) {
    auto insertion_start_point = inserted_element_coordinates->get_start_point();
    auto inserted_element = inserted_element_coordinates->get_sizes();
    auto space_height=get_height()-inserted_element.get_height();
    if(space_height==0){
        return nullptr;
    }
    return make_shared<Container::Free_Space>(Point_3D(insertion_start_point.get_x(),insertion_start_point.get_y()+inserted_element.get_height(),insertion_start_point.get_z()),
                                              inserted_element.get_width(),inserted_element.get_depth(),
                                              space_height,owner);
}

list<std::shared_ptr<Container::Free_Space>> Container::Free_Space::add_free_spaces_on_sides_of_inserted_element(A_Insertion_Coordinates *inserted_element_coordinates) {
    list<std::shared_ptr<Container::Free_Space>> created_free_spaces;
    auto insertion_end_point= inserted_element_coordinates->get_end_point();
    auto first=get_slice_on_back_from(insertion_end_point.get_z());
    auto second=get_slice_on_right_from(insertion_end_point.get_x());
    if(first!= nullptr && second!= nullptr){
        add_relation_between_free_spaces(first,second);
    }
    if(first!= nullptr){
        created_free_spaces.emplace_back(first);
    }
    if(second!= nullptr){
        created_free_spaces.emplace_back(second);
    }
    return created_free_spaces;
}

list<shared_ptr<Container::Free_Space>>Container::Free_Space::create_free_space_from_related_free_spaces(A_Insertion_Coordinates * inserted_element_coordinates) {
    list<shared_ptr<Container::Free_Space>> new_free_spaces;
    auto insertion_wider_point= inserted_element_coordinates->get_wider_point();
    auto insertion_deeper_point=inserted_element_coordinates->get_deeper_point();
    auto insertion_end_point=inserted_element_coordinates->get_end_point();
    auto it=related_free_spaces.begin();
    while(it!=related_free_spaces.end()){
        bool related_free_space_affected=false;
        shared_ptr<Container::Free_Space> first, second, third;
        if((*it)->do_point_affects(insertion_end_point)){
            first=(*it)->get_slice_on_right_from(insertion_end_point.get_x());
            second=(*it)->get_slice_on_back_from(insertion_end_point.get_z());

            related_free_space_affected=true;
        }
        if((*it)->do_point_affects(insertion_wider_point)){
            third=(*it)->get_slice_on_front_from(insertion_wider_point.get_z());
        }
        if((*it)->do_point_affects(insertion_deeper_point)){
            third=(*it)->get_slice_on_left_from(insertion_deeper_point.get_x());
        }

        if(related_free_space_affected){
            add_relation_between_free_spaces(first,second);
            add_relation_between_free_spaces(first,third);
            add_relation_between_free_spaces(second,third);
            if(first!= nullptr){
                new_free_spaces.emplace_back(first);
            }
            if(second!= nullptr){
                new_free_spaces.emplace_back(second);
            }
            if (third!=nullptr){
                new_free_spaces.emplace_back(third);
            }

            auto next=it;
            next++;
            owner->remove_free_space((*it));
            it=next;
        }
        else{
            it++;
        }
    }
    return new_free_spaces;
}

std::string Container::Free_Space::get_properties() const {
    auto properties=External_Free_Space::get_properties()+ " Related free spaces: ";
    for (const auto& related_free_space: related_free_spaces){
        properties+=std::to_string(related_free_space->get_id())+' ';
    }
    return properties;
}
