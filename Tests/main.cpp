//
// Created by kubam on 27.03.2024.
//

#include <catch2/catch_test_macros.hpp>
#include "Box.h"
#include "Test.h"

Test test;

TEST_CASE("Calculate volume"){
    Element_3D element(1,2,3);
    REQUIRE(element.get_volume()==6);
}

TEST_CASE("Rotate Insertable Element"){
    REQUIRE(test.test_insertable_element_rotation());
}

TEST_CASE("Get Insertable Element Rotation"){
    Box element(1,2,3);
    auto rotated= element.get_element_rotated_in_y();
    REQUIRE(rotated->get_width()==element.get_depth());
    REQUIRE(rotated->get_depth()==element.get_width());
}

TEST_CASE("Check Block Creation") {
    Test test;
    test.test_blocks_creation();
}