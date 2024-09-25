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
    std::string create_json() override;
};


#endif //MASTERS_WORK_BLOCK_INSERTION_COORDINATES_H
