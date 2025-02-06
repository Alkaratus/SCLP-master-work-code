#include "Packer.h"

#include <algorithm>
#include <utility>
#include <iostream>

using std::list, std::shared_ptr, std::vector;


void get_next_possible_block_elements_numbers(Block_Elements_Numbers &current, unsigned int max_number_of_elements);
list<shared_ptr<Simple_Block>> create_all_combinations_of_elements_blocks_for_numbers(const Elements_Group& group,Block_Elements_Numbers block_elements_numbers);
unsigned long series_sum(unsigned long last_word);
constexpr unsigned int FILL_SCALE=100;

Packer::Packer(const list<Box>& boxes, const Container& container):A_Packer(convert_boxes_to_elements(boxes),container){
    create_blocks();
}



list<std::unique_ptr<A_Insertion_Coordinates>> Packer::pack() {
    list<std::unique_ptr<A_Insertion_Coordinates>> packing_coordinates;
    while((!get_elements().empty())&&(get_container().have_free_space_available())){
        auto free_space_iterator=get_container().select_free_space();
        auto free_space=*(*free_space_iterator);
        if(auto element= select_element(free_space); element!= nullptr){
            packing_coordinates.emplace_back(get_container().insert_element_into_free_space(free_space_iterator,element));
            delete_element(element);
        }
        else{
            get_container().remove_free_space(*free_space_iterator);
        }
        std::cout<<get_container().get_text_list_of_free_spaces()<<std::endl;
    }
    return packing_coordinates;
}

Insertable_Element *Packer::select_element(Free_Space &selected_free_space){
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

void Packer::create_blocks() {
    get_elements().sort(compare_elements_ptr_by_lengths);
    auto groups=group_elements_in_list(get_elements());
    auto simple_blocks=create_simple_blocks(groups);
    auto complex_blocks= create_complex_block(groups,get_container());
    get_elements().insert(get_elements().end(),simple_blocks.begin(),simple_blocks.end());
    get_elements().insert(get_elements().end(),complex_blocks.begin(),complex_blocks.end());
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
            auto deleted_element=*it;
            auto deref_box= *box;
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
    list<list<std::shared_ptr<Insertable_Element>>::iterator> elements_to_delete;
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

list<Elements_Group>group_elements_in_list(const list<std::shared_ptr<Insertable_Element>> &elements) {
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

list<shared_ptr<Simple_Block>>create_simple_blocks(const list<Elements_Group> &groups){
    list<shared_ptr<Simple_Block>> possible_simple_blocks;
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

list<shared_ptr<Complex_Block>> create_complex_block(list<Elements_Group> groups, Container &container) {
    list<shared_ptr<Complex_Block>> possible_complex_blocks;

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
                    possible_complex_blocks.emplace_back(std::make_shared<Complex_Block>(block_elements,X));
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
                    possible_complex_blocks.emplace_back(std::make_shared<Complex_Block>(block_elements,Z));
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
                    possible_complex_blocks.emplace_back(std::make_shared<Complex_Block>(block_elements,Y));
                }

            }
            reached_end=increment_chain_of_iterators(iterators_chain,groups.end());
        }while(series_sum((*iterators_chain.begin())->get_group_element_properties().get_height()+iterators_chain.size()-1)<=container.get_height() && !reached_end);
        numbers_of_elements++;
        iterators_chain=create_chain_of_iterators(groups,numbers_of_elements);
    }

    return possible_complex_blocks;
}

unsigned long series_sum(unsigned long last_word) {
    return last_word*(last_word+1)/2;
}

bool compare_elements_ptr_by_lengths(const shared_ptr<Insertable_Element>& first, const shared_ptr<Insertable_Element>& second) {
    return compare_elements_by_lengths(*first,*second);
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

list<shared_ptr<Simple_Block>> create_all_combinations_of_elements_blocks_for_numbers(const Elements_Group& group, const Block_Elements_Numbers block_elements_numbers){
    list<std::shared_ptr<Simple_Block>> combinations;
    auto elements=group.get_elements_pointers();
    unsigned int i=0;
    bool width_and_depth_different=block_elements_numbers.get_elements_number_in_width()!=block_elements_numbers.get_elements_number_in_depth();
    bool depth_and_height_different=block_elements_numbers.get_elements_number_in_height()!=block_elements_numbers.get_elements_number_in_depth();
    do{
        auto slice=vector(elements.begin()+i,elements.begin()+i+block_elements_numbers.get_elements_number_in_block());

        combinations.emplace_back(std::make_shared<Simple_Block>(slice,block_elements_numbers));
        if(width_and_depth_different){
            combinations.emplace_back(std::make_shared<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_depth(),
                    block_elements_numbers.get_elements_number_in_width(),
                    block_elements_numbers.get_elements_number_in_height()))
            );
            combinations.emplace_back(std::make_shared<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_depth(),
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_width()))
            );
        }
        if(depth_and_height_different){
            combinations.emplace_back(std::make_shared<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_width(),
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_depth()))
            );
            combinations.emplace_back(std::make_shared<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_width(),
                    block_elements_numbers.get_elements_number_in_depth()))
            );
        }
        if(width_and_depth_different&&depth_and_height_different){
            combinations.emplace_back(std::make_shared<Simple_Block>(slice,Block_Elements_Numbers(
                    block_elements_numbers.get_elements_number_in_height(),
                    block_elements_numbers.get_elements_number_in_depth(),
                    block_elements_numbers.get_elements_number_in_width()))
            );
        }
        i+=block_elements_numbers.get_elements_number_in_block();
    }while(i+block_elements_numbers.get_elements_number_in_block()<=elements.size());

    return combinations;
}

std::vector<std::list<Elements_Group>::iterator>create_chain_of_iterators(std::list<Elements_Group> &groups, int elements_in_chain) {
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