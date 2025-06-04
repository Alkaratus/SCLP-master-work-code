//
// Created by kubam on 03.10.2024.
//

#include <utility>
#include "A_Packer.h"

#include <algorithm>

#include "Block_Elements_Numbers.h"
#include "Box.h"
#include "Complex_Block.h"
#include "Simple_Block.h"

using std::list, std::unique_ptr, std::vector;

void get_next_possible_block_elements_numbers(Block_Elements_Numbers &current, unsigned int max_number_of_elements);
list<unique_ptr<Simple_Block>> create_all_combinations_of_elements_blocks_for_numbers(const Elements_Group& group,Block_Elements_Numbers block_elements_numbers);
unsigned long series_sum(unsigned long last_word);

A_Packer::A_Packer(const std::list<std::unique_ptr<Insertable_Element>>& _elements,const Container& container):
container(container) {
    for(const auto &element : _elements) {
        elements.push_back(element->get_element_copy());
    }
    create_elements_rotations();
}

A_Packer::A_Packer(const A_Packer &other):container(other.container){
    for (const auto& element: other.elements) {
        elements.push_back(element->get_element_copy());
    }
}

A_Packer::A_Packer(const A_Packer *other):container(other->container) {
    for (const auto& element: other->elements) {
        elements.push_back(element->get_element_copy());
    }
}


void A_Packer::create_elements_rotations() {
    list<unique_ptr<Insertable_Element>>rotations;
    for(auto const &element:elements){
        auto rotated= element->get_element_rotated_in_y();
        if((*rotated)!=(*element) && (!container.cant_element_be_inserted(rotated.get()))){
            rotations.emplace_back(std::move(rotated));
        }
    }
    elements.merge(std::move(rotations));
}

list<unique_ptr<Insertable_Element>> & A_Packer::get_elements() {
    return elements;
}

Container& A_Packer::get_container() {
    return container;
}

void A_Packer::set_elements_groups(const std::list<Elements_Group> &_elements_groups) {
    elements_groups = _elements_groups;
}

const Container & A_Packer::get_container_data() const {
    return container;
}

const std::list<Elements_Group> & A_Packer::get_elements_groups_data() const {
    return elements_groups;
}

list<unique_ptr<Insertable_Element>> convert_boxes_to_elements(const list<Box>& boxes) {
    std::list<std::unique_ptr<Insertable_Element>> elements;
    for(auto &box:boxes){
        elements.emplace_back(std::make_unique<Box>(box));
    }
    return elements;
}


list<Elements_Group>group_elements_in_list(const list<std::unique_ptr<Insertable_Element>> &elements) {
    list<Elements_Group> groups;
    auto compared_element= elements.begin();
    list<Insertable_Element*>elements_IDs;
    elements_IDs.emplace_back(compared_element->get());
    for(auto iter=(++elements.begin());iter!=elements.end();iter++){
        if((**compared_element)!=(**iter)){
            groups.emplace_back(elements_IDs);
            elements_IDs.clear();
            compared_element=iter;
        }
        elements_IDs.emplace_back(iter->get());
    }
    groups.emplace_back(elements_IDs);
    return groups;
}

list<unique_ptr<Simple_Block>>create_simple_blocks(const list<Elements_Group> &groups){
    list<unique_ptr<Simple_Block>> possible_simple_blocks;
    for(auto &group:groups){
        const auto& group_elements_IDs=group.get_elements_pointers();
        Block_Elements_Numbers block_elements_numbers(2,1,1);
        while(block_elements_numbers.get_elements_number_in_block()<=group_elements_IDs.size()){
            auto combinations=create_all_combinations_of_elements_blocks_for_numbers(group,block_elements_numbers);
            possible_simple_blocks.insert( possible_simple_blocks.cend(),std::make_move_iterator(combinations.begin()),
                                         std::make_move_iterator(combinations.end()));
            get_next_possible_block_elements_numbers(block_elements_numbers,group_elements_IDs.size());
        }
    }
    return possible_simple_blocks;
}

list<unique_ptr<Complex_Block>> create_complex_block(list<Elements_Group> groups, Container &container) {
    list<unique_ptr<Complex_Block>> possible_complex_blocks;
    groups.sort([](const Elements_Group& a,const Elements_Group& b) {
        return compare_3D_elements_by_width(a.get_group_element_properties(),b.get_group_element_properties());
    });
    int numbers_of_elements=2;
    auto iterators_chain=create_chain_of_iterators(groups,numbers_of_elements);
    bool reached_end;
    while(series_sum((*iterators_chain.begin())->get_group_element_properties().get_width()+iterators_chain.size()-1)<=container.get_width()) {
        do {
            auto depth=(*iterators_chain.begin())->get_group_element_properties().get_depth();
            auto height=(*iterators_chain.begin())->get_group_element_properties().get_height();
            const bool all_have_same_wall=std::all_of(iterators_chain.begin(),iterators_chain.end(),[&depth,&height](list<Elements_Group>::iterator &group) {
                return group->get_group_element_properties().get_depth()==depth&&group->get_group_element_properties().get_height()==height;
            });
            if(all_have_same_wall) {
                auto max_number_of_groups=(*std::min_element(iterators_chain.begin(),iterators_chain.end(),
                    [](const list<Elements_Group>::iterator &first, const list<Elements_Group>::iterator &second){return first->get_elements_pointers().size()<second->get_elements_pointers().size();}))->get_elements_pointers().size();
                for(auto i=0;i<max_number_of_groups;i++) {
                    vector<Insertable_Element*>block_elements(iterators_chain.size());
                    for (auto j=0;j<iterators_chain.size();j++) {
                        block_elements[j]=iterators_chain[j]->get_elements_pointers()[0];
                    }
                    possible_complex_blocks.emplace_back(std::make_unique<Complex_Block>(block_elements,X));
                }

            }
            reached_end=increment_chain_of_iterators(iterators_chain,groups.end());
        }while(series_sum((*iterators_chain.begin())->get_group_element_properties().get_width()+iterators_chain.size()-1)<=container.get_width() && !reached_end);
        numbers_of_elements++;
        iterators_chain=create_chain_of_iterators(groups,numbers_of_elements);
    }

    groups.sort([](const Elements_Group& a,const Elements_Group& b) {
        return compare_3D_elements_by_depth(a.get_group_element_properties(),b.get_group_element_properties());
    });
    numbers_of_elements=2;
    iterators_chain=create_chain_of_iterators(groups,numbers_of_elements);
    while(series_sum((*iterators_chain.begin())->get_group_element_properties().get_depth()+iterators_chain.size()-1)<=container.get_depth()) {
        do {
            auto width=(*iterators_chain.begin())->get_group_element_properties().get_width();
            auto height=(*iterators_chain.begin())->get_group_element_properties().get_height();
            const bool all_have_same_wall=std::all_of(iterators_chain.begin(),iterators_chain.end(),[&width,&height](list<Elements_Group>::iterator &group) {
                return group->get_group_element_properties().get_width()==width&&group->get_group_element_properties().get_height()==height;
            });
            if(all_have_same_wall) {
                auto max_number_of_groups=(*std::min_element(iterators_chain.begin(),iterators_chain.end(),
                    [](const list<Elements_Group>::iterator &first, const list<Elements_Group>::iterator &second){return first->get_elements_pointers().size()<second->get_elements_pointers().size();}))->get_elements_pointers().size();
                for(auto i=0;i<max_number_of_groups;i++) {
                    vector<Insertable_Element*>block_elements(iterators_chain.size());
                    for (auto j=0;j<iterators_chain.size();j++) {
                        block_elements[j]=iterators_chain[j]->get_elements_pointers()[0];
                    }
                    possible_complex_blocks.emplace_back(std::make_unique<Complex_Block>(block_elements,Z));
                }

            }
            reached_end=increment_chain_of_iterators(iterators_chain,groups.end());
        }while(series_sum((*iterators_chain.begin())->get_group_element_properties().get_depth()+iterators_chain.size()-1)<=container.get_depth() && !reached_end);
        numbers_of_elements++;
        iterators_chain=create_chain_of_iterators(groups,numbers_of_elements);
    }

    groups.sort([](const Elements_Group& a,const Elements_Group& b) {
        return compare_3D_elements_by_height(a.get_group_element_properties(),b.get_group_element_properties()) ;
    });
    numbers_of_elements=2;
    iterators_chain=create_chain_of_iterators(groups,numbers_of_elements);
    while(series_sum((*iterators_chain.begin())->get_group_element_properties().get_depth()+iterators_chain.size()-1)<=container.get_depth()) {
        do {
            auto width=(*iterators_chain.begin())->get_group_element_properties().get_width();
            auto depth=(*iterators_chain.begin())->get_group_element_properties().get_depth();
            const bool all_have_same_wall=std::all_of(iterators_chain.begin(),iterators_chain.end(),[&width,&depth](list<Elements_Group>::iterator &group) {
                return group->get_group_element_properties().get_width()==width&&group->get_group_element_properties().get_depth()==depth;
            });
            if(all_have_same_wall) {
                auto max_number_of_groups=(*std::min_element(iterators_chain.begin(),iterators_chain.end(),
                    [](const list<Elements_Group>::iterator &first, const list<Elements_Group>::iterator &second){return first->get_elements_pointers().size()<second->get_elements_pointers().size();}))->get_elements_pointers().size();
                for(auto i=0;i<max_number_of_groups;i++) {
                    vector<Insertable_Element*>block_elements(iterators_chain.size());
                    for (auto j=0;j<iterators_chain.size();j++) {
                        block_elements[j]=iterators_chain[j]->get_elements_pointers()[0];
                    }
                    possible_complex_blocks.emplace_back(std::make_unique<Complex_Block>(block_elements,Y));
                }

            }
            reached_end=increment_chain_of_iterators(iterators_chain,groups.end());
        }while(series_sum((*iterators_chain.begin())->get_group_element_properties().get_height()+iterators_chain.size()-1)<=container.get_height() && !reached_end);
        numbers_of_elements++;
        iterators_chain=create_chain_of_iterators(groups,numbers_of_elements);
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

unsigned long series_sum(unsigned long last_word) {
    return last_word*(last_word+1)/2;
}

list<unique_ptr<Simple_Block>> create_all_combinations_of_elements_blocks_for_numbers(const Elements_Group& group, const Block_Elements_Numbers block_elements_numbers){
    list<std::unique_ptr<Simple_Block>> combinations;
    auto elements=group.get_elements_pointers();
    unsigned int i=0;
    bool width_and_depth_different=block_elements_numbers.get_elements_number_in_width()!=block_elements_numbers.get_elements_number_in_depth();
    bool depth_and_height_different=block_elements_numbers.get_elements_number_in_height()!=block_elements_numbers.get_elements_number_in_depth();
    do{
        auto slice=vector(elements.begin()+i,elements.begin()+i+block_elements_numbers.get_elements_number_in_block());

        combinations.emplace_back(std::make_unique<Simple_Block>(slice,block_elements_numbers));
        if(width_and_depth_different){
            combinations.emplace_back(std::make_unique<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_depth(),
                    block_elements_numbers.get_elements_number_in_width(),
                    block_elements_numbers.get_elements_number_in_height()))
            );
            combinations.emplace_back(std::make_unique<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_depth(),
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_width()))
            );
        }
        if(depth_and_height_different){
            combinations.emplace_back(std::make_unique<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_width(),
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_depth()))
            );
            combinations.emplace_back(std::make_unique<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_width(),
                    block_elements_numbers.get_elements_number_in_depth()))
            );
        }
        if(width_and_depth_different&&depth_and_height_different){
            combinations.emplace_back(std::make_unique<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_depth(),
                    block_elements_numbers.get_elements_number_in_width()))
            );
        }
        i+=block_elements_numbers.get_elements_number_in_block();
    }while(i+block_elements_numbers.get_elements_number_in_block()<=elements.size());
    return combinations;
}

std::vector<std::list<Elements_Group>::iterator>create_chain_of_iterators(std::list<Elements_Group> &groups, const int elements_in_chain) {
    std::vector<std::list<Elements_Group>::iterator> chain(elements_in_chain);
    auto element=groups.begin();
    for(int i=0;i<elements_in_chain;i++) {
        chain[i]=element++;
    }
    return chain;
}

bool increment_chain_of_iterators(vector<list<Elements_Group>::iterator>&iterators,list<Elements_Group>::iterator end) {
    auto first_incremented=iterators.size()-1;
    ++iterators[first_incremented];
    while(first_incremented>0 && iterators[iterators.size()-1]==end) {
        first_incremented--;
        ++iterators[first_incremented];
        auto next_incremented=first_incremented+1;
        while(next_incremented<iterators.size()) {
            iterators[next_incremented]=iterators[next_incremented-1];
            ++iterators[next_incremented];
            next_incremented++;
        }
    }
    return iterators[iterators.size()-1]==end;
}

bool compare_elements_ptr_by_lengths(const std::unique_ptr<Insertable_Element>& first, const std::unique_ptr<Insertable_Element>& second) {
    return compare_elements_by_lengths(*first,*second);
}