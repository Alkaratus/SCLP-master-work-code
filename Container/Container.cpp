//
// Created by kubam on 01.03.2024.
//

#include "Container.h"
#include <algorithm>
#include <iostream>

using std::list, std::shared_ptr, std::make_shared;

///Container

Container::Container(unsigned int width, unsigned int depth, unsigned int height):
        Element_3D(width,depth,height), element_max_sizes(width,depth,height){
    free_spaces.emplace_back(make_shared<Container::Free_Space>(Point_3D(0,0,0),width,depth,height,this));
}

Container::Container(const Container &other): Element_3D(other), element_max_sizes(other.element_max_sizes) {
    free_spaces.emplace_back(make_shared<Container::Free_Space>(Point_3D(0,0,0),get_width(),
                                                                get_depth(),get_height(),this));
}

void Container::remove_inserted_spaces() {
    auto it=free_spaces.begin();
    while (it!=free_spaces.end()){
        auto related_free_spaces=(*it)->get_related_free_spaces();
        bool is_inside_another=false;
        for(auto &related_free_space:related_free_spaces){
            if((*it)->is_inside_another(*related_free_space)){
                is_inside_another= true;
                auto list_of_related=related_free_space->get_related_free_spaces();
                for(auto &another_related_free_space:related_free_spaces){
                    if(another_related_free_space!=related_free_space){
                        if(std::find(list_of_related.begin(),list_of_related.end(),another_related_free_space)==list_of_related.end()){
                            add_relation_between_free_spaces(related_free_space,another_related_free_space);
                        }
                    }
                }
            }
        }
        if(is_inside_another){
            auto next=it;
            next++;
            remove_free_space(*it);
            it=next;
        }
        else{
            it++;
        }
    }
}

list<shared_ptr<Container::Free_Space>>::iterator Container::select_free_space() {
    return free_spaces.begin();
}

void Container::add_relation_between_free_spaces(const shared_ptr<Free_Space>& first, const shared_ptr<Free_Space>& second) {
    if(first==nullptr || second==nullptr){
        return;
    }
    first->add_related_free_space(second);
    second->add_related_free_space(first);
}

bool Container::compare_free_spaces_anchors_lengths(shared_ptr<Container::Free_Space> &first,shared_ptr<Container::Free_Space> &second) {
    return is_first_anchor_smaller_than_second(first->get_anchor_lengths(),second->get_anchor_lengths());
}

bool Container::compare_free_spaces_by_y_coordinate(shared_ptr<Container::Free_Space> &first,
                                                    shared_ptr<Container::Free_Space> &last) {
    return first->get_anchor_corner().get_y() < last->get_anchor_corner().get_y();
}

void Container::mark_relations_between_free_spaces_in_lists(list<std::shared_ptr<Free_Space>> &first,
                                                            list<std::shared_ptr<Free_Space>> &second) {
    auto it=first.begin();
    auto jt=second.begin();
    while (it!=first.end()){
        while(jt!=second.end()){
            if((*it)->is_related_with_another(**jt)){
                add_relation_between_free_spaces(*it,*jt);
            }
            jt++;
        }
        it++;
        jt=second.begin();
    }
}

void Container::remove_inserted_spaces_from_list(list<std::shared_ptr<Free_Space>> &merged_free_spaces) {
    auto it=merged_free_spaces.begin();
    auto jt=merged_free_spaces.begin();
    jt++;
    while(it!=merged_free_spaces.end()){
        while(jt!=merged_free_spaces.end()){
            if(jt->get()->is_inside_another(**it)){
                for(auto &element:merged_free_spaces){
                    element->remove_related_free_space(*jt);
                }
                for(auto &element:free_spaces){
                    element->remove_related_free_space(*jt);
                }
                jt=merged_free_spaces.erase(jt);
            }
            else if(it->get()->is_inside_another(**jt)){
                for(auto &element:merged_free_spaces){
                    element->remove_related_free_space(*it);
                }
                for(auto &element:free_spaces){
                    element->remove_related_free_space(*it);
                }
                it=merged_free_spaces.erase(it);
                jt=it;
                jt++;
            }
            else{
                jt++;
            }
        }
        it++;
        jt=it;
        jt++;
    }

}

list<shared_ptr<Container::Free_Space>>::iterator Container::get_first_element_with_y_coordinate(unsigned int y) {
    auto first_free_space=free_spaces.begin();
    while(first_free_space!=free_spaces.end() && first_free_space->get()->get_height()!=y){
        first_free_space++;
    }
    return first_free_space;
}

list<shared_ptr<Container::Free_Space>>::iterator
Container::get_last_element_with_same_y_after(list<shared_ptr<Free_Space>>::iterator first) {
    auto last_free_space=first;
    while(last_free_space != free_spaces.end() && first->get()->get_height() == last_free_space->get()->get_height()){
        last_free_space++;
    }
    return last_free_space;
}

Insertion_Coordinates Container::insert_element_into_free_space(list<shared_ptr<Container::Free_Space>>::iterator free_space,
                                               Insertable_Element *element) {
    auto insertion_coordinates=free_space->get()->get_insertion_coordinates_for_element(element);
    auto free_space_above=free_space->get()->create_free_space_above_inserted_element(insertion_coordinates);

    auto free_spaces_on_sides=free_space->get()->add_free_spaces_on_sides_of_inserted_element(insertion_coordinates);
    auto related_free_spaces=free_space->get()->create_free_space_from_related_free_spaces(insertion_coordinates);

    remove_free_space(*free_space);

    if(free_space_above!= nullptr){
        free_spaces.sort(compare_free_spaces_by_y_coordinate);
        make_merges_for_new(free_space_above);
    }
    mark_relations_between_free_spaces_in_lists(free_spaces,free_spaces_on_sides);
    mark_relations_between_free_spaces_in_lists(free_spaces,related_free_spaces);
    mark_relations_between_free_spaces_in_lists(free_spaces_on_sides,related_free_spaces);

    free_spaces.insert(free_spaces.end(),related_free_spaces.begin(),related_free_spaces.end());
    free_spaces.insert(free_spaces.end(),free_spaces_on_sides.begin(),free_spaces_on_sides.end());
    remove_inserted_spaces();

    free_spaces.sort(compare_free_spaces_anchors_lengths);
    return insertion_coordinates;
}

bool Container::cant_element_be_inserted(Insertable_Element* element) {
    return element_max_sizes.get_width()<element->get_width()||
           element_max_sizes.get_depth()<element->get_depth()||
           element_max_sizes.get_height()<element->get_height();
}

void Container::remove_free_space(shared_ptr<Free_Space> &space) {
    for(auto it=free_spaces.begin();it!=free_spaces.end();it++){
        if(it->get()==space.get()){
            auto related_free_spaces=it->get()->get_related_free_spaces();
            for(const auto& related_free_space:related_free_spaces){
                related_free_space->remove_related_free_space(*it);
            }
            free_spaces.erase(it);
            return;
        }
    }
}

std::string Container::get_text_list_of_free_spaces() {
    std::string list;
    for (const auto& free_space:free_spaces){
        list+=free_space->get_properties()+'\n';
    }
    return list;
}

bool Container::have_free_space_aveable() {
    return (!free_spaces.empty());
}

void Container::make_merges_for_new(std::shared_ptr<Free_Space> &space) {
    auto first_free_space= get_first_element_with_y_coordinate(space->get_height());
    if(first_free_space == free_spaces.end()){
        free_spaces.emplace_back(space);
        return;
    }

    auto last_free_space= get_last_element_with_same_y_after(first_free_space);

    std::list<shared_ptr<Free_Space>> new_free_spaces, merged_free_spaces;
    new_free_spaces.emplace_back(space);
    auto iter=first_free_space;
    while(!new_free_spaces.empty()){
        for(auto &new_space : new_free_spaces){
            while(iter != last_free_space){
                auto new_free_space=create_merge_in_x(new_space,*iter);
                if(new_free_space){
                    for(auto iter2=first_free_space;iter2!=last_free_space;iter2++){
                        if(new_free_space->is_related_with_another(**iter2)){
                            add_relation_between_free_spaces(*iter2,new_free_space);
                        }
                    }
                    for(auto &free_space: new_free_spaces){
                        if(new_free_space->is_related_with_another(*free_space)){
                            add_relation_between_free_spaces(free_space,new_free_space);
                        }
                    }
                    for(auto &free_space: merged_free_spaces){
                        if(new_free_space->is_related_with_another(*free_space)){
                            add_relation_between_free_spaces(free_space,new_free_space);
                        }
                    }
                    merged_free_spaces.emplace_back(new_free_space);
                }
                new_free_space=create_merge_in_z(new_space,*iter);
                if(new_free_space){
                    for(auto iter2=first_free_space;iter2!=last_free_space;iter2++){
                        if(new_free_space->is_related_with_another(**iter2)){
                            add_relation_between_free_spaces(*iter2,new_free_space);
                        }
                    }
                    for(auto &free_space: new_free_spaces){
                        if(new_free_space->is_related_with_another(*free_space)){
                            add_relation_between_free_spaces(free_space,new_free_space);
                        }
                    }
                    for(auto &free_space: merged_free_spaces){
                        if(new_free_space->is_related_with_another(*free_space)){
                            add_relation_between_free_spaces(free_space,new_free_space);
                        }
                    }
                    merged_free_spaces.emplace_back(new_free_space);
                }
                iter++;
            }
            iter=first_free_space;
            last_free_space= get_last_element_with_same_y_after(first_free_space);
        }

        auto it=merged_free_spaces.begin();
        while(it!=merged_free_spaces.end()){
            bool duplicate_or_inserted=false;
            //Sprawdzenie duplikatów
            for(auto iter2=first_free_space;iter2!=last_free_space;iter2++){
                if(it->get()->is_inside_another(**iter2)){
                    duplicate_or_inserted=true;
                }
            }
            for(auto &free_space: new_free_spaces){
                if(it->get()->is_inside_another(*free_space)){
                    duplicate_or_inserted=true;
                }
            }
            if(duplicate_or_inserted){
                for(auto &element:free_spaces){
                    element->remove_related_free_space(*it);
                }
                for(auto &element:merged_free_spaces){
                    element->remove_related_free_space(*it);
                }
                for(auto &free_space: new_free_spaces){
                    free_space->remove_related_free_space(*it);
                }
                it=merged_free_spaces.erase(it);
            }
            else{
                it++;
            }
        }

        //Sprawdzenie czy scalone przestrzenie nie zawierają się w sobie
        free_spaces.insert(last_free_space,new_free_spaces.begin(),new_free_spaces.end());
        remove_inserted_spaces_from_list(merged_free_spaces);
        first_free_space= get_first_element_with_y_coordinate(space->get_height());
        last_free_space= get_last_element_with_same_y_after(first_free_space);
        new_free_spaces=merged_free_spaces;
        merged_free_spaces.clear();
    }


}

shared_ptr<Container::Free_Space> Container::create_merge_in_x(const shared_ptr<Free_Space> &first, const shared_ptr<Free_Space> &second) {
    if(first->is_adjacent_to_another_in_x(*second)){
        auto start_corner=first->get_start_corner();
        auto end_corner= first->get_end_corner();
        auto other_start_corner=second->get_start_corner();
        auto other_end_corner=second->get_end_corner();
        auto merged=make_shared<Free_Space>(Point_3D(start_corner.get_x(),start_corner.get_y(),std::max(start_corner.get_z(),other_start_corner.get_z())),
                                            other_end_corner.get_x()-start_corner.get_x(),
                                            std::min(end_corner.get_z(),other_end_corner.get_z())-std::max(start_corner.get_z(),other_start_corner.get_z()),
                                            first->get_height(),first->get_owner());
        return merged;
    }
    if(second->is_adjacent_to_another_in_x(*first)){
        auto start_corner=second->get_start_corner();
        auto end_corner= second->get_end_corner();
        auto other_start_corner=first->get_start_corner();
        auto other_end_corner=first->get_end_corner();
        auto merged=make_shared<Free_Space>(Point_3D(start_corner.get_x(),start_corner.get_y(),std::max(start_corner.get_z(),other_start_corner.get_z())),
                                       other_end_corner.get_x()-start_corner.get_x(),
                                       std::min(end_corner.get_z(),other_end_corner.get_z())-std::max(start_corner.get_z(),other_start_corner.get_z()),
                                       first->get_height(),first->get_owner());
        return merged;
    }
    return nullptr;
}

shared_ptr<Container::Free_Space>Container::create_merge_in_z(const shared_ptr<Free_Space> &first, const shared_ptr<Free_Space> &second) {
    if(first->is_adjacent_to_another_in_z(*second)){
        auto start_corner=first->get_start_corner();
        auto end_corner= first->get_end_corner();
        auto other_start_corner=second->get_start_corner();
        auto other_end_corner=second->get_end_corner();
        return make_shared<Free_Space>(Point_3D(std::max(start_corner.get_x(),other_start_corner.get_x()),start_corner.get_y(),start_corner.get_z()),
                                       std::min(end_corner.get_x(),other_end_corner.get_x()-std::max(start_corner.get_x(),other_start_corner.get_x())),
                                       other_end_corner.get_z()-start_corner.get_z(),
                                       first->get_height(),first->get_owner());
    }
    if(second->is_adjacent_to_another_in_z(*first)){
        auto start_corner=second->get_start_corner();
        auto end_corner= second->get_end_corner();
        auto other_start_corner=first->get_start_corner();
        auto other_end_corner=first->get_end_corner();
        return make_shared<Free_Space>(Point_3D(std::max(start_corner.get_x(),other_start_corner.get_x()),start_corner.get_y(),start_corner.get_z()),
                                       std::min(end_corner.get_x(),other_end_corner.get_x())-std::max(start_corner.get_x(),other_start_corner.get_x()),
                                       other_end_corner.get_z()-start_corner.get_z(),
                                       first->get_height(),first->get_owner());
    }
    return nullptr;
}


/// Internal functions



bool is_first_anchor_smaller_than_second(std::array<unsigned int,3> first, std::array<unsigned int,3> second){
    if(first[0]!=second[0]){
        return first[0]<second[0];
    }
    if(first[1]!=second[1]){
        return first[1]<second[1];
    }
    return first[2]<second[2];
}

