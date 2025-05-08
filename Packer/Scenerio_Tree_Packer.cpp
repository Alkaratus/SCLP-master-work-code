//
// Created by kubam on 16.12.2024.
//

#include "Scenerio_Tree_Packer.h"

#include <algorithm>
#include <iostream>
#include <utility>

#include "Box.h"
#include "Complex_Block.h"
#include "Simple_Block.h"

using std::list, std::pair;


Scenerio_Tree_Packer::Scenerio_Tree_Packer(const std::list<Box> &boxes, const Container &container,
                                           unsigned int number_of_levels, std::vector<unsigned int> scenerios_in_level):
A_Packer(convert_boxes_to_elements(boxes),container),levels_number(number_of_levels),scenerios_number_in_level(std::move(scenerios_in_level)) {
    create_blocks();
}

Scenerio_Tree_Packer::Scenerio_Tree_Packer(Scenerio_Tree_Packer &other):A_Packer(other),levels_number(other.levels_number),
scenerios_number_in_level(other.scenerios_number_in_level){
}

Insertable_Element * Scenerio_Tree_Packer::get_element_by_id(const unsigned int id) {
    for(const auto& element:get_elements()) {
        if(element->get_id() == id) {
            return element.get();
        }
    }
    return nullptr;
}

list<std::pair<std::unique_ptr<A_Insertion_Coordinates>,int>> Scenerio_Tree_Packer::pack_one_element(unsigned int scenerio_level) {
    if(!get_container().have_free_space_available()) {
        return {};
    }
    auto free_space_iterator=get_container().select_free_space();
    auto free_space=*(*free_space_iterator);
    auto elements_ids=select_elements_to_pack_ids(free_space,scenerio_level);

    if(elements_ids.empty()) {
        get_container().remove_free_space(*free_space_iterator);
        return {};
    }
    // tworzenie packerów do prób
    std::vector<std::unique_ptr<Scenerio_Tree_Packer>> test_packers(scenerios_number_in_level[scenerio_level]);
    for(unsigned int i=0;i<scenerios_number_in_level[scenerio_level];i++) {
        test_packers[i]=std::make_unique<Scenerio_Tree_Packer>(*this);
    }

    //Tworzenie scenariuszy
    list<list<std::pair<std::unique_ptr<A_Insertion_Coordinates>,int>>> scenerios;
    for (unsigned int i=0;i<elements_ids.size();i++) {
        list<std::pair<std::unique_ptr<A_Insertion_Coordinates>,int>> insertions;
        auto selected_free_space=test_packers[i]->get_container().select_free_space();
        auto element_to_pack=test_packers[i]->get_element_by_id(elements_ids[i].first);

        auto free_space_volume=selected_free_space->get()->get_volume();
        auto element_volume= element_to_pack->get_volume();
        auto mark= element_volume- volume_lost_ratio*(free_space_volume -element_volume);
        auto rotated= element_to_pack->get_element_rotated_in_y();
        if(!(*selected_free_space)->can_element_be_inserted(element_to_pack)) {
            element_to_pack=rotated.get();
        }
        if(scenerio_level+1!=levels_number) {
            auto insertion=test_packers[i]->get_container().insert_element_into_free_space(selected_free_space,element_to_pack);

            insertions.emplace_back(std::move(insertion),mark);
            test_packers[i]->delete_element(element_to_pack);
            auto next_elements=test_packers[i]->pack_one_element(scenerio_level+1);
            for(auto &element : next_elements) {
                insertions.emplace_back(std::move(element));
            }
        }
        else {
            insertions.emplace_back(selected_free_space->get()->get_insertion_coordinates_for_element(element_to_pack),mark);
        }
        scenerios.emplace_back(std::move(insertions));
    }

    //Porównanie scenariuszy
    auto iterator_on_best=std::max_element(scenerios.begin(),scenerios.end(),[](const list<std::pair<std::unique_ptr<A_Insertion_Coordinates>,int>> &first,
    const list<std::pair<std::unique_ptr<A_Insertion_Coordinates>,int>> &second) {
        unsigned sum_first=0, sum_second=0;
        for(auto & it : first) {
            sum_first+=it.second;
        }
        for(auto & it :second) {
            sum_second+=it.second;
        }
        return sum_first<sum_second;
    });

    return std::move(*iterator_on_best);
}

void Scenerio_Tree_Packer::create_blocks() {
    get_elements().sort(compare_elements_ptr_by_lengths);
    auto groups=group_elements_in_list(get_elements());
    auto simple_blocks=create_simple_blocks(groups);
    auto complex_blocks= create_complex_block(groups,get_container());
    for(auto &simple_block:simple_blocks) {
        get_elements().push_back(std::move(simple_block));
    }
    for(auto &complex_block:complex_blocks) {
        get_elements().push_back(std::move(complex_block));
    }
    set_elements_groups(groups);
}


list<std::unique_ptr<A_Insertion_Coordinates>> Scenerio_Tree_Packer::pack() {
    //std::cout<<get_container().get_text_list_of_free_spaces()<<std::endl;
    list<std::unique_ptr<A_Insertion_Coordinates>> packing_coordinates;
    while((!get_elements().empty())&&(get_container().have_free_space_available())) {
        auto best_scenerio=pack_one_element(0);
        auto selected_free_space=get_container().select_free_space();
        if(best_scenerio.empty()) {
            //std::cout<<get_container().get_text_list_of_free_spaces()<<std::endl;
            continue;
        }
        auto element_to_pack=get_element_by_id((*best_scenerio.begin()).first->get_element_id());
        auto rotated= element_to_pack->get_element_rotated_in_y();
        if(!(*selected_free_space)->can_element_be_inserted(element_to_pack)) {
            element_to_pack=rotated.get();
        }
        packing_coordinates.emplace_back(get_container().insert_element_into_free_space(selected_free_space,element_to_pack));
        delete_element(element_to_pack);
        //std::cout<<get_container().get_text_list_of_free_spaces()<<std::endl;
    }
    return packing_coordinates;
}

void Scenerio_Tree_Packer::visit(Box *box) {
    auto it=get_elements().begin();
    auto element_to_delete=get_elements().begin();
    while(it!=get_elements().end()){
        if(it->get()->contains_element_with_id(box->get_id())){
            it=get_elements().erase(it);
        }
        else if(it->get()->get_id()==box->get_id()){
            element_to_delete=it;
            it++;
        }
        else{
            it++;
        }
    }
    get_elements().erase(element_to_delete);
}

void Scenerio_Tree_Packer::visit(Simple_Block *block) {
    auto block_elements=block->get_block_elements_pointers();
    auto it=get_elements().begin();
    list<list<std::unique_ptr<Insertable_Element>>::iterator> elements_to_delete;
    while(it!=get_elements().end()){
        if(it->get()->contains_element_with_id(block->get_id())){
            it=get_elements().erase(it);
        }
        else if(it->get()->get_id()==block->get_id()){
            elements_to_delete.emplace_back(it);
            it++;
        }
        else{
            it++;
        }
    }
    for(auto iterator:elements_to_delete) {
        get_elements().erase(iterator);
    }
    for(auto &layer:block_elements){
        for(auto &row:layer){
            for(auto &element:row){
                delete_element(element);
            }
        }
    }
}

void Scenerio_Tree_Packer::visit(Complex_Block *block) {
    auto block_elements=block->get_block_elements_pointers();
    auto it=get_elements().begin();
    auto element_to_delete=get_elements().begin();
    while(it!=get_elements().end()){
        if(it->get()->contains_element_with_id(block->get_id())){
            it=get_elements().erase(it);
        }
        else if(it->get()->get_id()==block->get_id()){
            element_to_delete=it;
            ++it;
        }
        else{
            ++it;
        }
    }
    get_elements().erase(element_to_delete);
    for(auto &element:block_elements) {
        delete_element(element);
    }
}


std::vector<pair<unsigned int,unsigned int>> Scenerio_Tree_Packer::select_elements_to_pack_ids(Free_Space &selected_free_space,unsigned int level_number) {
    list<pair<unsigned int,unsigned int>> elements_id_and_fulfill;
    auto elements_it=get_elements().begin();
    unsigned int i=0;
    while(i<scenerios_number_in_level[level_number] && elements_it!=get_elements().end()) {
        auto fulfill=(*elements_it)->get_volume() * FILL_SCALE / selected_free_space.get_volume();
        if(selected_free_space.can_element_be_inserted((*elements_it).get())) {
            elements_id_and_fulfill.emplace_back((*elements_it)->get_id(),fulfill);
            ++i;
        }

        ++elements_it;
    }

    elements_id_and_fulfill.sort([](const pair<unsigned int,unsigned int> &first,const pair<unsigned int,unsigned int> &second) {
        return first.second < second.second;
    });

    for (;elements_it!=get_elements().end();++elements_it) {
        if(!selected_free_space.can_element_be_inserted((*elements_it).get())) {
            continue;
        }
        auto fulfill=(*elements_it)->get_volume() * FILL_SCALE / selected_free_space.get_volume();
        if(fulfill>FILL_SCALE || std::any_of(elements_id_and_fulfill.begin(),elements_id_and_fulfill.end(),[elements_it](const pair<unsigned int,unsigned int> &best_fulfill) {return best_fulfill.first==(*elements_it)->get_id();})){
            continue;
        }
        std::pair element_id_and_fulfill((*elements_it)->get_id(),fulfill);
        auto best_elements_it=elements_id_and_fulfill.begin();
        while(best_elements_it!=elements_id_and_fulfill.end()&&element_id_and_fulfill.second>best_elements_it->second) {
            ++best_elements_it;
        }
        if(best_elements_it!=elements_id_and_fulfill.begin()) {
            elements_id_and_fulfill.emplace((best_elements_it),element_id_and_fulfill);
            elements_id_and_fulfill.pop_front();
        }
    }

    return {elements_id_and_fulfill.begin(),elements_id_and_fulfill.end()};
}

void Scenerio_Tree_Packer::delete_element(Insertable_Element *element) {
    element->accept(this);
}
