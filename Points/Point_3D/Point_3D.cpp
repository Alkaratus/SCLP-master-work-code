//
// Created by kubam on 02.03.2024.
//

#include "Point_3D.h"

Point_3D::Point_3D(unsigned int x, unsigned int y, unsigned int z):
        Point_2D(x,z),y(y) {
}

unsigned int Point_3D::get_y() const {
    return y;
}

Point_3D& Point_3D::operator=(const Point_3D& other) = default;

void Point_3D::set_y(unsigned int _y) {
    y=_y;
}

bool Point_3D::operator==(const Point_3D &other) {
    return Point_2D::operator==(other) && y==other.y;
}

