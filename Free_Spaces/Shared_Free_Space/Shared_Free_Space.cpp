//
// Created by kubam on 03.03.2024.
//

#include "Shared_Free_Space.h"

#include <utility>

Shared_Free_Space::Shared_Free_Space(std::vector<Free_Space *> free_spaces, Point_2D start, unsigned int width,
                                     unsigned int depth): free_spaces(std::move(free_spaces)), start(start),
                                     width(width),depth(depth){
}

bool Shared_Free_Space::is_point_in(Point_2D &point) {
    return start.get_x()<point.get_x() && point.get_x()<start.get_x()+width &&
            start.get_z()<point.get_z() && point.get_z()<start.get_z()+depth;
}
