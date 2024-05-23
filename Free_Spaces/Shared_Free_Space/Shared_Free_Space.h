//
// Created by kubam on 03.03.2024.
//

#ifndef MASTERS_WORK_SHARED_FREE_SPACE_H
#define MASTERS_WORK_SHARED_FREE_SPACE_H

#include <vector>
#include "Point_2D.h"

class Free_Space;

class Shared_Free_Space{
    std::vector<Free_Space*> free_spaces;
    Point_2D start;
    unsigned int width;
    unsigned int depth;
public:
    Shared_Free_Space(std::vector<Free_Space*> free_spaces,Point_2D start,unsigned int width,unsigned int depth);
    bool is_point_in(Point_2D &point);
};


#endif //MASTERS_WORK_SHARED_FREE_SPACE_H
