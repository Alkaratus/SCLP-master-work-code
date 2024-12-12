//
// Created by kubam on 21.09.2024.
//

#include "Insertion_Coordinates_Factory.h"
#include "Box_Insertion_Coordinates.h"
#include "Simple_Block.h"
#include "Block_Insertion_Coordinates.h"
#include "Complex_Block.h"

void Insertion_Coordinates_Factory::visit(Box *box) {
    created_insertion_coordinate= std::make_unique<Box_Insertion_Coordinates>(start_point,box);
}

void Insertion_Coordinates_Factory::visit(Simple_Block *block) {
    std::vector<std::unique_ptr<A_Insertion_Coordinates>> sub_elements;
    auto elements_set=block->get_block_elements_pointers();
    for(int i=0;i<elements_set.size();i++){
        for(int j=0;j<elements_set[i].size();j++){
            for(int k=0;k<elements_set[i][j].size();k++){
                Insertion_Coordinates_Factory another_factory;
                auto element_start_point=start_point;
                element_start_point.set_x(start_point.get_x()+elements_set[i][j][k]->get_width()*k);
                element_start_point.set_z(start_point.get_z()+elements_set[i][j][k]->get_depth()*j);
                element_start_point.set_y(start_point.get_y()+elements_set[i][j][k]->get_depth()*i);
                sub_elements.emplace_back(another_factory.create_insertion_coordinate(elements_set[i][j][k],element_start_point));
            }
        }
    }
    created_insertion_coordinate= std::make_unique<Block_Insertion_Coordinates>(start_point,block,std::move(sub_elements));
}

void Insertion_Coordinates_Factory::visit(Complex_Block *block) {
    std::vector<std::unique_ptr<A_Insertion_Coordinates>> sub_elements;
    const auto elements_set=block->get_block_elements_pointers();
    unsigned int sum=0;
    switch(block->get_merge_axis()) {
        case X: {
            for(const auto element:elements_set) {
                Insertion_Coordinates_Factory another_factory;
                auto element_start_point=start_point;
                element_start_point.set_x(start_point.get_x()+sum);
                sum+=element->get_width();
                sub_elements.emplace_back(another_factory.create_insertion_coordinate(element,element_start_point));
            }
            break;
        }
        case Z: {
            for(auto element:elements_set) {
                Insertion_Coordinates_Factory another_factory;
                auto element_start_point=start_point;
                element_start_point.set_z(start_point.get_z()+sum);
                sum+=element->get_depth();
                sub_elements.emplace_back(another_factory.create_insertion_coordinate(element,element_start_point));
            }
            break;
        }
        case Y: {
            for(auto element:elements_set) {
                Insertion_Coordinates_Factory another_factory;
                auto element_start_point=start_point;
                element_start_point.set_y(start_point.get_y()+sum);
                sum+=element->get_height();
                sub_elements.emplace_back(another_factory.create_insertion_coordinate(element,element_start_point));
            }
            break;
        }
    }
    created_insertion_coordinate= std::make_unique<Block_Insertion_Coordinates>(start_point,block,std::move(sub_elements));
}

std::unique_ptr<A_Insertion_Coordinates>
Insertion_Coordinates_Factory::create_insertion_coordinate(Insertable_Element *element, Point_3D point) {
    start_point=point;
    element->accept(this);
    return std::move(created_insertion_coordinate);
}
