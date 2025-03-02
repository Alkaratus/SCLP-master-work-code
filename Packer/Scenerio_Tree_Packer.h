//
// Created by kubam on 16.12.2024.
//

#ifndef SCENERIO_TREE_PACKER_H
#define SCENERIO_TREE_PACKER_H
#include <vector>

#include "A_Packer.h"


class Scenerio_Tree_Packer: public A_Packer {
    unsigned int levels_number;
    std::vector<unsigned int> scenerios_number_in_level;

    std::vector<std::pair<unsigned int,unsigned int>> select_elements_to_pack_ids(Free_Space &selected_free_space,unsigned int level_number);
    Insertable_Element* get_element_by_id(unsigned int id);

    std::list<std::unique_ptr<A_Insertion_Coordinates>>pack_one_element(unsigned int scenerio_level);

    void create_blocks();
    void delete_element(Insertable_Element *element);

public:
    Scenerio_Tree_Packer(const std::list<Box>& boxes, const Container& container, unsigned int number_of_scenerios, std::vector<unsigned int> scenerios_in_level={1});
    Scenerio_Tree_Packer(Scenerio_Tree_Packer &other);
    std::list<std::unique_ptr<A_Insertion_Coordinates>> pack() override;

    void visit(Box *box) override;
    void visit(Simple_Block *block) override;
    void visit(Complex_Block *block) override;
};



#endif //SCENERIO_TREE_PACKER_H
