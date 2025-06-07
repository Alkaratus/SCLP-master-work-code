//
// Created by kubam on 03.06.2025.
//

#include "Parallel_Tree_Packer.h"
#include "Box.h"
#include "Complex_Block.h"
#include "Packer.h"
#include "Simple_Block.h"

using std::list,std::pair,std::unique_ptr;

Parallel_Tree_Packer::Parallel_Tree_Packer(const std::list<Box> &boxes, const Container &container, std::vector<unsigned int> scenerios_in_level):
A_Packer(convert_boxes_to_elements(boxes),container),scenerios_in_level(std::move(scenerios_in_level)){
    create_blocks();
}

Insertable_Element * Parallel_Tree_Packer::get_element_by_id(const unsigned int id) {
    for(const auto& element:get_elements()) {
        if(element->get_id() == id) {
            return element.get();
        }
    }
    return nullptr;
}

std::list<std::unique_ptr<A_Insertion_Coordinates>> Parallel_Tree_Packer::pack() {
    list<std::unique_ptr<A_Insertion_Coordinates>> packing_coordinates;
    auto scenerios=get_scenerios(0,{});
    for(auto &scenerio:scenerios) {
        auto results=scenerio.first->pack_naive();
        scenerio.second.insert(scenerio.second.end(),std::make_move_iterator(results.begin()),std::make_move_iterator(results.end()));
    }
    auto best_scenerio=scenerios.begin();
    double best_container_usage=0;
    for(auto it=scenerios.begin();it!=scenerios.end();++it) {
        auto container_usage=calculate_container_usage(get_container(),it->second);
        if(container_usage>best_container_usage) {
            best_container_usage=container_usage;
            best_scenerio=it;
        }
    }
    return std::move(best_scenerio->second);
}

list<pair<unique_ptr<Parallel_Tree_Packer>,list<unique_ptr<A_Insertion_Coordinates>>>> Parallel_Tree_Packer::get_scenerios(
    unsigned int level, const list<unique_ptr<A_Insertion_Coordinates>>& previous) {
    list<pair<unique_ptr<Parallel_Tree_Packer>,list<unique_ptr<A_Insertion_Coordinates>>>> scenerios;
    auto free_space_iterator=get_container().select_free_space();
    auto free_space=*(*free_space_iterator);
    auto elements_ids=select_elements_to_pack_ids(free_space,level);
    std::vector<unique_ptr<Parallel_Tree_Packer>> test_packers(scenerios_in_level[level]);
    for(unsigned int i=0;i<scenerios_in_level[level];++i) {
        test_packers[i]=std::make_unique<Parallel_Tree_Packer>(*this);
        auto element_to_pack=test_packers[i]->get_element_by_id(elements_ids[i].first);
        auto rotated= element_to_pack->get_element_rotated_in_y();
        auto selected_free_space=get_container().select_free_space();
        if(!(*selected_free_space)->can_element_be_inserted(element_to_pack)) {
            element_to_pack=rotated.get();
        }
        auto insertion=test_packers[i]->get_container().insert_element_into_free_space(test_packers[i]->get_container().select_free_space(),element_to_pack);
        test_packers[i]->delete_element(element_to_pack);
        auto new_previous=create_copy(previous);
        new_previous.emplace_back(std::move(insertion));
        if(level<scenerios_in_level.size()-1) {
            auto new_scenerios= test_packers[i]->get_scenerios(level+1,new_previous);
            scenerios.insert(scenerios.end(),std::make_move_iterator(new_scenerios.begin()),std::make_move_iterator(new_scenerios.end()));
        }else {
            scenerios.emplace_back(std::move(test_packers[i]),std::move(new_previous));
        }
    }
    return scenerios;
}

std::list<std::unique_ptr<A_Insertion_Coordinates>> Parallel_Tree_Packer::pack_naive() {
    list<std::unique_ptr<A_Insertion_Coordinates>> packing_coordinates;
    while((!get_elements().empty())&&(get_container().have_free_space_available())){
        auto free_space_iterator=get_container().select_free_space();
        auto free_space=*(*free_space_iterator);
        if(auto element= select_element_by_volume(free_space); element!= nullptr){
            packing_coordinates.emplace_back(get_container().insert_element_into_free_space(free_space_iterator,element));
            delete_element(element);
        }
        else{
            get_container().remove_free_space(*free_space_iterator);
        }

    }
    return packing_coordinates;
}

Insertable_Element * Parallel_Tree_Packer::select_element_by_volume(const Free_Space &selected_free_space) {
    Insertable_Element* best_fill_element=nullptr;
    unsigned int best_fill_ratio=0;
    for(auto &element:get_elements()){
        if(selected_free_space.can_element_be_inserted(element.get())){
            auto fill_ratio= element->get_volume() * FILL_SCALE / selected_free_space.get_volume();
            if(fill_ratio > best_fill_ratio){
                best_fill_ratio=fill_ratio;
                best_fill_element=element.get();
            }
        }
    }
    return best_fill_element;
}


void Parallel_Tree_Packer::create_blocks() {
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

void Parallel_Tree_Packer::delete_element(Insertable_Element *element) {
    element->accept(this);
}

void Parallel_Tree_Packer::visit(Box *box) {
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

void Parallel_Tree_Packer::visit(Simple_Block *block) {
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

void Parallel_Tree_Packer::visit(Complex_Block *block) {
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

std::vector<pair<unsigned int,unsigned int>> Parallel_Tree_Packer::select_elements_to_pack_ids(Free_Space &selected_free_space,unsigned int level_number) {
    list<pair<unsigned int,unsigned int>> elements_id_and_fulfill;
    auto elements_it=get_elements().begin();
    unsigned int i=0;
    while(i<scenerios_in_level[level_number] && elements_it!=get_elements().end()) {
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


