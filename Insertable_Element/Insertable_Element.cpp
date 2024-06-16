//
// Created by kubam on 29.02.2024.
//


#include "Insertable_Element.h"

unsigned int Insertable_Element::next_element_id=0;

Insertable_Element::Insertable_Element(unsigned int width, unsigned int depth, unsigned int height):
Element_3D(width,depth,height),id(++next_element_id) {
}

Insertable_Element::Insertable_Element(const Insertable_Element &other):id(other.id),Element_3D(other) {
}


void Insertable_Element::rotate() {
    set_width(get_width()^get_depth());
    set_depth(get_width()^get_depth());
    set_width(get_width()^get_depth());
}

unsigned int Insertable_Element::get_id() const{
    return id;
}

std::string Insertable_Element::get_properties() const {
    return "ID: "+std::to_string(id)+' '+Element_3D::get_properties();
}

void Insertable_Element::accept(Visitor *packer) {

}

bool operator==(const Insertable_Element &first, const Insertable_Element &second){
    return first.get_width()==second.get_width()&&
    first.get_depth()==second.get_depth()&&
    first.get_height()==second.get_height();
}

bool operator!=(const Insertable_Element &first, const Insertable_Element &second) {
    return first.get_width()!=second.get_width()||
           first.get_depth()!=second.get_depth()||
           first.get_height()!=second.get_height();
}

bool compare_elements_by_id(const Insertable_Element &first, const Insertable_Element &second) {
    return first.get_id()<second.get_id();
}

bool compare_elements_by_lengths(const Insertable_Element &first, const Insertable_Element &second) {
    if(first.get_width()!=second.get_width()){
        return first.get_width()<second.get_width();
    }
    if(first.get_depth()!=second.get_depth()){
        return first.get_depth()<second.get_depth();
    }
    return first.get_height()<second.get_height();
}

