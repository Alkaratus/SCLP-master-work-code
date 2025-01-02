//
// Created by kubam on 16.12.2024.
//

#ifndef SCENERIO_TREE_PACKER_H
#define SCENERIO_TREE_PACKER_H
#include <vector>

#include "A_Packer.h"


class Scenerio_Tree_Packer: public A_Packer {
    std::vector<unsigned int> scenerios_in_level;
    public:
    Scenerio_Tree_Packer(const std::list<Box>& boxes, const Container& container, std::vector<unsigned int> scenerios_in_level={1});
    std::list<std::unique_ptr<A_Insertion_Coordinates>> pack() override;
};



#endif //SCENERIO_TREE_PACKER_H
