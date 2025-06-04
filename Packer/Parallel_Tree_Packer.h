//
// Created by kubam on 03.06.2025.
//

#ifndef PARALLEL_TREE_PACKER_H
#define PARALLEL_TREE_PACKER_H
#include "A_Packer.h"


class Parallel_Tree_Packer: public A_Packer {

    std::vector<unsigned int> scenerios_in_level;

    void create_blocks();
    void delete_element(Insertable_Element *element);
    std::vector<std::pair<unsigned int,unsigned int>> select_elements_to_pack_ids(Free_Space &selected_free_space,unsigned int level_number);

    std::list<std::pair<std::unique_ptr<Parallel_Tree_Packer>,std::list<std::unique_ptr<A_Insertion_Coordinates>>>> get_scenerios(unsigned int level,
        const std::list<std::unique_ptr<A_Insertion_Coordinates>>& previous);
public:
    enum Element_Selecting_Method {
        by_max_volume,
        by_max_surface,
        by_first_satisfying_element
    };

    Parallel_Tree_Packer(const std::list<Box>& boxes, const Container& container, std::vector<unsigned int> scenerios_in_level={1});

    Insertable_Element *get_element_by_id(unsigned int id);

    std::list<std::unique_ptr<A_Insertion_Coordinates>> pack() override;

    void visit(Box *box) override;
    void visit(Simple_Block *block) override;
    void visit(Complex_Block *block) override;
};




#endif //PARALLEL_TREE_PACKER_H
