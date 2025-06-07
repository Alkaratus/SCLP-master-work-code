//
// Created by kubam on 18.09.2024.
//

#ifndef MASTERS_WORK_BLOCK_INSERTION_COORDINATES_H
#define MASTERS_WORK_BLOCK_INSERTION_COORDINATES_H


#include <vector>
#include "A_Insertion_Coordinates.h"
#include "Block.h"

class Block_Insertion_Coordinates: public A_Insertion_Coordinates{
    unsigned int element_id;
    std::vector<std::unique_ptr<A_Insertion_Coordinates>> sub_elements;
public:
    Block_Insertion_Coordinates(Point_3D startPoint, Block *block, std::vector<std::unique_ptr<A_Insertion_Coordinates>> sub_elements);
    Block_Insertion_Coordinates(const Block_Insertion_Coordinates &other);
    std::string accept(I_Coordinates_Displayer* displayer) override;

    unsigned int get_element_id() const;
    std::vector<std::unique_ptr<A_Insertion_Coordinates>>& get_sub_elements();
    std::unique_ptr<A_Insertion_Coordinates>create_copy() const override;
};


#endif //MASTERS_WORK_BLOCK_INSERTION_COORDINATES_H
