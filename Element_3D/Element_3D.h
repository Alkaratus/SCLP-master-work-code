//
// Created by kubam on 01.03.2024.
//

#ifndef MASTERS_WORK_ELEMENT_3D_H
#define MASTERS_WORK_ELEMENT_3D_H

#include <string>

class Element_3D {
    unsigned int width;
    unsigned int depth;
    unsigned int height;
protected:
    void set_width(unsigned int width);
    void set_depth(unsigned int depth);
    void set_height(unsigned int height);
public:
    Element_3D(unsigned int width, unsigned int depth, unsigned int height);
    unsigned int get_width() const;
    unsigned int get_depth() const;
    unsigned int get_height() const;
    unsigned int get_volume() const;

    bool operator==(const Element_3D &other)const;
    virtual std::string get_properties() const;
};
bool compare_3D_elements_by_volume(const Element_3D& first, const Element_3D& second);
bool compare_3D_elements_lexically(const Element_3D& first, const Element_3D& second);

#endif //MASTERS_WORK_ELEMENT_3D_H
