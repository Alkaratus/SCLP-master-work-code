//
// Created by kubam on 02.04.2024.
//

#include "Test.h"
#include "Insertable_Element.h"

bool Test::test_insertable_element_rotation() {
    Insertable_Element element(3,2,1);
    auto width_before_rotation=element.get_width();
    auto depth_before_rotation= element.get_depth();
    auto height_before_rotation=element.get_height();
    element.rotate();
    return width_before_rotation==element.get_depth() && depth_before_rotation==element.get_width() &&
    height_before_rotation==element.get_height();
}
