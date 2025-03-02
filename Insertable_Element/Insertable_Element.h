//
// Created by kubam on 29.02.2024.
//

#ifndef MASTERS_WORK_INSERTABLE_ELEMENT_H
#define MASTERS_WORK_INSERTABLE_ELEMENT_H

class Visitor;

#include <memory>
#include "Element_3D.h"

class Insertable_Element: public Element_3D {
    static unsigned int next_element_id;
    unsigned int id;
protected:
    ///Creates copy instance of element for element rotation creation
    Insertable_Element(const Insertable_Element &other);
public:
    Insertable_Element(unsigned int width, unsigned int depth, unsigned int height);
    ~Insertable_Element() override =default;
    virtual void rotate_in_y()=0;
    /// Creates copy of element
    /// \return copied element
    virtual std::unique_ptr<Insertable_Element> get_element_copy()=0;
    virtual std::unique_ptr<Insertable_Element> get_element_rotated_in_y()=0;
    virtual bool contains_element_with_id(unsigned int id)=0;
    [[nodiscard]] unsigned int get_id() const;
    [[nodiscard]] std::string get_properties() const override;

    virtual void accept(Visitor *packer);
    friend class Test;
};

bool compare_elements_by_id(const Insertable_Element &first, const Insertable_Element &second);
bool compare_elements_by_lengths(const Insertable_Element &first, const Insertable_Element &second);

bool operator==(const Insertable_Element &first, const Insertable_Element &second);
bool operator!=(const Insertable_Element &first, const Insertable_Element &second);

#endif //MASTERS_WORK_INSERTABLE_ELEMENT_H
