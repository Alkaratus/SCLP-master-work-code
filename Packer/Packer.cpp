#include "Packer.h"

#include <iostream>
#include <map>
#include <stdexcept>

#include "Complex_Block.h"
#include "Simple_Block.h"
#include "Parallel_Tree_Packer.h"

using std::list, std::shared_ptr, std::vector;

const std::map<Packer::Element_Selecting_Method,std::function<Insertable_Element*(Packer*,Free_Space &)>> Packer::selection_methods={
    {by_max_volume,select_element_by_volume},
    {by_max_surface,select_element_by_surface},
    {by_first_satisfying_element,select_element_by_first_satisfying_element}
};

Packer::Packer(const list<Box>& boxes, const Container& container,Element_Selecting_Method method):A_Packer(convert_boxes_to_elements(boxes),container){

    create_blocks();
    select_method= selection_methods.at(method);
}

Packer::Packer(const Parallel_Tree_Packer *other):A_Packer(other) {
}

list<std::unique_ptr<A_Insertion_Coordinates>> Packer::pack() {
    list<std::unique_ptr<A_Insertion_Coordinates>> packing_coordinates;
    while((!get_elements().empty())&&(get_container().have_free_space_available())){
        auto free_space_iterator=get_container().select_free_space();
        auto free_space=*(*free_space_iterator);
        if(auto element= select_method(this,free_space); element!= nullptr){
            packing_coordinates.emplace_back(get_container().insert_element_into_free_space(free_space_iterator,element));
            delete_element(element);
        }
        else{
            get_container().remove_free_space(*free_space_iterator);
        }

    }
    return packing_coordinates;
}

std::unique_ptr<A_Insertion_Coordinates> Packer::pack_single_element(Insertable_Element *element) {
    auto free_space_iterator=get_container().select_free_space();
    return get_container().insert_element_into_free_space(free_space_iterator,element);
}

Insertable_Element * Packer::get_element_by_id(const unsigned int id) {
    for(const auto& element:get_elements()) {
        if(element->get_id() == id) {
            return element.get();
        }
    }
    return nullptr;
}

Insertable_Element *Packer::select_element_by_volume(const Free_Space &selected_free_space){
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

Insertable_Element * Packer::select_element_by_surface(const Free_Space &selected_free_space) {
    Insertable_Element* best_fill_element=nullptr;
    unsigned int best_fill_ratio=0;
    for(auto &element:get_elements()){
        if(selected_free_space.can_element_be_inserted(element.get())){
            auto fill_ratio= element->get_base_surface() * FILL_SCALE / selected_free_space.get_base_surface();
            if(fill_ratio > best_fill_ratio){
                best_fill_ratio=fill_ratio;
                best_fill_element=element.get();
            }
        }
    }
    return best_fill_element;
}

Insertable_Element * Packer::select_element_by_first_satisfying_element(const Free_Space &selected_free_space) {
    throw std::logic_error("This function is not implemented yet");
}


void Packer::create_blocks() {
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

void Packer::delete_element(Insertable_Element *element) {
    element->accept(this);
}

void Packer::visit(Box *box) {
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

void Packer::visit(Simple_Block *block) {
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

void Packer::visit(Complex_Block *block) {
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
