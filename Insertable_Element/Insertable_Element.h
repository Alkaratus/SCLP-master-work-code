//
// Created by kubam on 29.02.2024.
//

#ifndef MASTERS_WORK_INSERTABLE_ELEMENT_H
#define MASTERS_WORK_INSERTABLE_ELEMENT_H


#include <memory>
#include "Element_3D.h"

class Insertable_Element: public Element_3D {
    static unsigned int next_element_id;
    unsigned int id;
protected:
    ///Creates copy instance of element for element rotation creation
    Insertable_Element(const Insertable_Element &other);
    void rotate();
public:
    Insertable_Element(unsigned int width, unsigned int depth, unsigned int height);
    virtual std::shared_ptr<Insertable_Element> get_rotated_element()=0;
    [[nodiscard]] unsigned int get_id() const;
    [[nodiscard]] std::string get_properties() const override;
    virtual bool contains_element_with_id(unsigned int id)=0;

    friend class Test;
};

bool compare_elements_by_id(const Insertable_Element &first, const Insertable_Element &second);

bool operator==(const Insertable_Element &first, const Insertable_Element &second);
bool operator!=(const Insertable_Element &first, const Insertable_Element &second);

#endif //MASTERS_WORK_INSERTABLE_ELEMENT_H
