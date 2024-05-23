//
// Created by kubam on 08.03.2024.
//

#include "Packer.h"

#include <utility>
#include <climits>
#include <iostream>

using std::list;

void get_next_possible_block_elements_numbers(Block_Elements_Numbers &current, unsigned int max_number_of_elements);

list<Simple_Block> create_all_combinations_of_elements_blocks_for_numbers(const Elements_Group& group,Block_Elements_Numbers block_elements_numbers);

const unsigned int FILL_SCALE=100;
const unsigned int MIN_FILL_RATIO=100;

Packer::Packer(const std::list<Box>& boxes, const Container& container):container(container),
max_number_of_simple_blocks(UINT_MAX),max_number_of_complex_block_merges(UINT_MAX),min_fill_ratio(MIN_FILL_RATIO) {
    for(auto &box:boxes){
        elements.emplace_back(std::make_unique<Box>(box));
    }
    create_elements_rotations();
    //group_elements_in_list(elements);
}

void Packer::create_elements_rotations() {
    std::list<std::shared_ptr<Insertable_Element>>rotations;
    for(auto const &element:elements){
        auto rotated=element->get_rotated_element();
        if((*rotated)!=(*element) && (!container.cant_element_be_inserted(rotated.get()))){
            rotations.emplace_back(std::move(rotated));
        }
    }
    elements.merge(rotations);
    // TODO: we need kind of sorting function
}

void Packer::pack() {
    while((!elements.empty())&&(container.have_free_space_aveable())){
        auto free_space_iterator=container.select_free_space();
        auto free_space=*(*free_space_iterator);
        auto element= select_element(free_space);
        if(element!= nullptr){
            container.insert_element_into_free_space(free_space_iterator,element);
            delete_elements_with_id(element->get_id());
        }
        else{
            container.remove_free_space(*free_space_iterator);
        }
        std::cout<<container.get_text_list_of_free_spaces()<<std::endl;
    }
}

Insertable_Element *Packer::select_element(Free_Space &selected_free_space) const{
    Insertable_Element* best_fill_element=nullptr;
    unsigned int best_fill_ratio=0;
    for(auto &element:elements){
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

void Packer::delete_elements_containing_element_with_id(unsigned int id) {

}

void Packer::delete_elements_with_id(unsigned int id) {
    for (auto iterator=elements.begin();iterator!=elements.end();iterator++){
        if((*iterator)->get_id()==id){
            iterator=elements.erase(iterator);
        }
    }
}

std::list<Elements_Group>group_elements_in_list(const std::list<std::unique_ptr<Insertable_Element>> &elements) {
    list<Elements_Group> groups;
    auto compared_element= elements.begin();
    list<unsigned int>elements_IDs;
    elements_IDs.emplace_back((*compared_element)->get_id());
    for(auto iter=(++elements.begin());iter!=elements.end();iter++){
        if((*compared_element)!=(*iter)){
            //groups.emplace_back(elements_IDs,*compared_element);
            elements_IDs.clear();
            compared_element=iter;
        }
        elements_IDs.emplace_back((*iter)->get_id());
    }
    return groups;
}

list<Simple_Block>create_simple_blocks(const list<Elements_Group> &groups){
    list<Simple_Block> possible_simple_blocks;
    for(auto &group:groups){
        const auto& group_elements_IDs=group.get_elements_pointers();
        Block_Elements_Numbers block_elements_numbers(2,1,1);
        while(block_elements_numbers.get_elements_number_in_block()<group_elements_IDs.size()){
            //auto combinations=create_all_combinations_of_elements_blocks_for_numbers(group,block_elements_numbers);
            //possible_simple_blocks.insert( possible_simple_blocks.cbegin(),std::make_move_iterator(combinations.begin()),
            //                               std::make_move_iterator(combinations.end()));
            get_next_possible_block_elements_numbers(block_elements_numbers,group_elements_IDs.size());
        }
    }
    return possible_simple_blocks;
}

list<Complex_Block> create_complex_block(const list<Elements_Group> &groups) {
    list<Complex_Block> possible_complex_blocks;
    for(auto first=groups.begin();first!=groups.end();first++){
        auto first_properties= first->get_group_element_properties();
        auto second=first;
        second++;
        for(;second!=groups.end();second++){
            auto second_properties=second->get_group_element_properties();
            auto widths_the_same=first_properties.get_width()==second_properties.get_width();
            auto depths_the_same=first_properties.get_depth()==second_properties.get_depth();
            auto heights_the_same=first_properties.get_height()==second_properties.get_height();
            if(widths_the_same&&depths_the_same){
                possible_complex_blocks.emplace_back(Complex_Block({},axis::Y));
            }
            else if(widths_the_same&&heights_the_same){
                possible_complex_blocks.emplace_back(Complex_Block({},axis::Z));
            }
            else if(depths_the_same&&heights_the_same){
                possible_complex_blocks.emplace_back(Complex_Block({},axis::X));
            }
        }
    }
    return possible_complex_blocks;
}

void get_next_possible_block_elements_numbers(Block_Elements_Numbers &current, unsigned int max_number_of_elements) {
    current.set_elements_number_in_width(current.get_elements_number_in_width()+1);
    if(current.get_elements_number_in_block()>max_number_of_elements){
        current.set_elements_number_in_depth(current.get_elements_number_in_depth()+1);
        current.set_elements_number_in_width(current.get_elements_number_in_depth());
    }
    if(current.get_elements_number_in_block()>max_number_of_elements){
        current.set_elements_number_in_height(current.get_elements_number_in_height()+1);
        current.set_elements_number_in_depth(current.get_elements_number_in_height());
        current.set_elements_number_in_width(current.get_elements_number_in_height());
    }
}

/*list<Simple_Block> create_all_combinations_of_elements_blocks_for_numbers(const Elements_Group& group,Block_Elements_Numbers block_elements_numbers){
    list<Simple_Block> combinations;
    auto elements=group.get_elements_pointers();
    auto slice=std::vector(elements.begin(),elements.begin()+block_elements_numbers.get_elements_number_in_block());

    combinations.emplace_back(std::make_shared<Simple_Block>(slice,block_elements_numbers));
    bool width_and_depth_different=block_elements_numbers.get_elements_number_in_width()!=block_elements_numbers.get_elements_number_in_depth();
    bool depth_and_height_different=block_elements_numbers.get_elements_number_in_height()!=block_elements_numbers.get_elements_number_in_depth();
    if(width_and_depth_different){
        combinations.emplace_back(slice,Block_Elements_Numbers(
                block_elements_numbers.get_elements_number_in_depth(),
                block_elements_numbers.get_elements_number_in_width(),
                block_elements_numbers.get_elements_number_in_height())
        );
        combinations.emplace_back(slice,Block_Elements_Numbers(
                block_elements_numbers.get_elements_number_in_depth(),
                block_elements_numbers.get_elements_number_in_height(),
                block_elements_numbers.get_elements_number_in_width())
        );
    }
    if(depth_and_height_different){
        combinations.emplace_back(slice,Block_Elements_Numbers(
                block_elements_numbers.get_elements_number_in_width(),
                block_elements_numbers.get_elements_number_in_height(),
                block_elements_numbers.get_elements_number_in_depth())
        );
        combinations.emplace_back(slice,Block_Elements_Numbers(
                block_elements_numbers.get_elements_number_in_height(),
                block_elements_numbers.get_elements_number_in_width(),
                block_elements_numbers.get_elements_number_in_depth())
        );
    }
    if(width_and_depth_different&&depth_and_height_different){
        combinations.emplace_back(slice,Block_Elements_Numbers(
                block_elements_numbers.get_elements_number_in_height(),
                block_elements_numbers.get_elements_number_in_depth(),
                block_elements_numbers.get_elements_number_in_width())
        );
    }
    return combinations;
}*/
