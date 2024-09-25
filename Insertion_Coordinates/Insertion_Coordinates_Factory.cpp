//
// Created by kubam on 21.09.2024.
//

#include "Insertion_Coordinates_Factory.h"
#include "Box_Insertion_Coordinates.h"
#include "Simple_Block.h"
#include "Block_Insertion_Coordinates.h"

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
                element_start_point.set_x(element_start_point.get_x()+elements_set[i][j][k]->get_width()*k);
                element_start_point.set_z(element_start_point.get_z()+elements_set[i][j][k]->get_depth()*j);
                element_start_point.set_y(element_start_point.get_y()+elements_set[i][j][k]->get_depth()*i);
                sub_elements.emplace_back(another_factory.create_insertion_coordinate(elements_set[i][j][k],element_start_point));
            }
        }
    }
    created_insertion_coordinate= std::make_unique<Block_Insertion_Coordinates>(start_point,block,std::move(sub_elements));
}

void Insertion_Coordinates_Factory::visit(Complex_Block *block) {

}

std::unique_ptr<A_Insertion_Coordinates>
Insertion_Coordinates_Factory::create_insertion_coordinate(Insertable_Element *element, Point_3D point) {
    start_point=point;
    element->accept(this);
    return std::move(created_insertion_coordinate);
}
