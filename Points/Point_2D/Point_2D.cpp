//
// Created by kubam on 02.03.2024.
//

#include "Point_2D.h"

Point_2D::Point_2D(unsigned int x, unsigned int z):
x(x),z(z) {
}

unsigned int Point_2D::get_x() const {
    return x;
}

unsigned int Point_2D::get_z() const {
    return z;
}

Point_2D& Point_2D::operator=(const Point_2D& other) = default;

void Point_2D::set_x(unsigned int _x) {
    x=_x;
}

void Point_2D::set_z(unsigned int _z) {
    z=_z;
}

bool Point_2D::operator==(const Point_2D &other) const {
    return x==other.x && z==other.z;
}

