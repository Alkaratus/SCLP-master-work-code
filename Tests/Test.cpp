//
// Created by kubam on 02.04.2024.
//

#include "Test.h"
#include "Box.h"
#include "Packer.h"
#include "Scenerio_Tree_Packer.h"
#include "Trials.h"
#include <catch2/catch_test_macros.hpp>


bool Test::test_insertable_element_rotation() {
    Box element(3,2,1);
    auto width_before_rotation=element.get_width();
    auto depth_before_rotation= element.get_depth();
    auto height_before_rotation=element.get_height();
    element.rotate_in_y();
    return width_before_rotation==element.get_depth() && depth_before_rotation==element.get_width() &&
    height_before_rotation==element.get_height();
}

bool Test::test_blocks_creation() {
    auto data=read_file("br1.txt");
    auto [container,boxes]=data[1];
    Packer packer(boxes,container,Packer::by_max_volume);
    Scenerio_Tree_Packer tree_packer(boxes,container,2,{2,2});
    auto &packer_elements=packer.get_elements();
    auto &tree_elements=tree_packer.get_elements();
    REQUIRE(packer_elements.size()==tree_elements.size());
    auto packer_iter=packer_elements.begin();
    auto tree_iter=tree_elements.begin();
    for(int i=0;i<packer_elements.size();i++) {
        auto &packer_element=*packer_iter;
        auto &tree_element=*tree_iter;
        REQUIRE(packer_element->get_width()==tree_element->get_width());
        REQUIRE(packer_element->get_depth()==tree_element->get_depth());
        REQUIRE(packer_element->get_height()==tree_element->get_height());
        ++packer_iter;
        ++tree_iter;
    }
    return true;
}
