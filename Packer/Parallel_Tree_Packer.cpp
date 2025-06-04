//
// Created by kubam on 03.06.2025.
//

#include "Parallel_Tree_Packer.h"
#include "Box.h"
#include "Complex_Block.h"
#include "Packer.h"
#include "Simple_Block.h"

using std::list,std::pair;

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
    for(int i=0;i<scenerios_in_level.size();i++) {
        std::vector<std::unique_ptr<Packer>> scenerio_packers;
        scenerio_packers.emplace_back(std::make_unique<Packer>(this));
        auto free_space_iterator=get_container().select_free_space();
        auto free_space=*(*free_space_iterator);
        auto elements_ids=select_elements_to_pack_ids(free_space,i);
        std::vector<std::unique_ptr<Packer>> test_packers(scenerios_in_level[i]);
        for(unsigned int j=0;i<scenerios_in_level[j];i++) {
            test_packers[j]=std::make_unique<Packer>(this);
            auto element_to_pack=test_packers[j]->get_element_by_id(elements_ids[j].first);

        }
    }
    return packing_coordinates;
}

list<pair<std::unique_ptr<Parallel_Tree_Packer>,list<std::unique_ptr<A_Insertion_Coordinates>>>> Parallel_Tree_Packer::get_scenerios(
    unsigned int level, const list<std::unique_ptr<A_Insertion_Coordinates>>& previous) {
    //TODO dokończyć i przetestować
    list<pair<std::unique_ptr<Parallel_Tree_Packer>,list<std::unique_ptr<A_Insertion_Coordinates>>>> scenerios;
    auto free_space_iterator=get_container().select_free_space();
    auto free_space=*(*free_space_iterator);
    auto elements_ids=select_elements_to_pack_ids(free_space,level);
    std::vector<std::unique_ptr<Parallel_Tree_Packer>> test_packers(scenerios_in_level[level]);
    for(unsigned int i=0;i<scenerios_in_level[i];i++) {
        test_packers[i]=std::make_unique<Parallel_Tree_Packer>(*this);
        auto element_to_pack=test_packers[i]->get_element_by_id(elements_ids[i].first);
        auto rotated= element_to_pack->get_element_rotated_in_y();
        auto selected_free_space=get_container().select_free_space();
        if(!(*selected_free_space)->can_element_be_inserted(element_to_pack)) {
            element_to_pack=rotated.get();
        }
        auto insertion=test_packers[i]->get_container().insert_element_into_free_space(selected_free_space,element_to_pack);
        auto new_previous=previous;
        new_previous.emplace_back(std::move(insertion));
        if(level<scenerios_in_level.size()-1) {
            auto new_scenerios= test_packers[i]->get_scenerios(level+1,new_previous);
            scenerios.insert(scenerios.end(),new_scenerios.begin(),new_scenerios.end());
        }
    }
    return scenerios;
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


