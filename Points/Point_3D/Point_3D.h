//
// Created by kubam on 02.03.2024.
//

#ifndef MASTERS_WORK_POINT_3D_H
#define MASTERS_WORK_POINT_3D_H

#include "Point_2D.h"

class Point_3D:public Point_2D {
    unsigned int y;
public:
    explicit Point_3D(unsigned int x=0, unsigned int y=0, unsigned int z=0);
    unsigned int get_y() const;

    void set_y(unsigned int y);

    Point_3D& operator=(const Point_3D& other);
    bool operator==(const Point_3D& other);
};




#endif //MASTERS_WORK_POINT_3D_H
