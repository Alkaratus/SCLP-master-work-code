//
// Created by kubam on 01.03.2024.
//
#include "Element_3D.h"

Element_3D::Element_3D(const unsigned int width, const unsigned int depth, const unsigned int height):
        width(width),depth(depth),height(height){
}

void Element_3D::set_width(unsigned int _width) {
    width=_width;
}

void Element_3D::set_depth(unsigned int _depth) {
    depth=_depth;
}

void Element_3D::set_height(unsigned int _height) {
    height=_height;
}

unsigned int Element_3D::get_width() const {
    return width;
}

unsigned int Element_3D::get_depth() const {
    return depth;
}

unsigned int Element_3D::get_height() const {
    return height;
}

unsigned int Element_3D::get_volume() const {
    return width*depth*height;
}

std::string Element_3D::get_properties() const {
    return "Width:"+std::to_string(width)+" Depth: "+std::to_string(depth)+
    " Height:"+std::to_string(height);
}

bool Element_3D::operator==(const Element_3D &other) const {
    return width==other.width && depth==other.depth && height==other.height;
}

bool compare_3D_elements_by_volume(const Element_3D& first, const Element_3D& second) {
    return first.get_volume()<second.get_volume();
}

bool compare_3D_elements_lexically(const Element_3D& first, const Element_3D& second) {
    return first.get_width()==second.get_width()?
            first.get_depth()==second.get_depth()?
                first.get_height()<second.get_height()
                :first.get_depth()<second.get_depth()
            :first.get_width()<second.get_width();
}

bool compare_3D_elements_by_width(const Element_3D& first, const Element_3D& second) {
    return compare_3D_elements_lexically(first,second);
}

bool compare_3D_elements_by_depth(const Element_3D& first, const Element_3D& second) {
    return first.get_depth()==second.get_depth()?
            first.get_width()==second.get_width()?
                first.get_height()<second.get_height()
                :first.get_width()<second.get_width()
            :first.get_depth()<second.get_depth();
}

bool compare_3D_elements_by_height(const Element_3D& first, const Element_3D& second) {
    return first.get_height()==second.get_height()?
            first.get_depth()==second.get_depth()?
                first.get_width()<second.get_width()
                :first.get_depth()<second.get_depth()
            :first.get_height()<second.get_height();
}
