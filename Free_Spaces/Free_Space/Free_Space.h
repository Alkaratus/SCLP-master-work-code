//
// Created by kubam on 02.03.2024.
//

#ifndef MASTERS_WORK_FREE_SPACE_H
#define MASTERS_WORK_FREE_SPACE_H

#include <list>
#include <memory>
#include "Point_3D.h"
#include "Insertable_Element.h"

class Shared_Free_Space;

class Free_Space:public Element_3D {
    static unsigned int next_id;
    unsigned int id;
    Point_3D start_point;
public:
    [[nodiscard]] Point_3D get_start_corner() const;
    [[nodiscard]] Point_3D get_wider_corner() const;
    [[nodiscard]] Point_3D get_deeper_corner() const;
    [[nodiscard]] Point_3D get_end_corner() const;

    Free_Space(Point_3D start, unsigned int width, unsigned int depth, unsigned int height);
    bool can_element_be_inserted(const Insertable_Element* element);
    unsigned int get_id();
    [[nodiscard]] bool is_point_deep_inside(const Point_2D &point) const;
    [[nodiscard]] bool is_point_inside(const Point_2D &point) const;
    std::string get_properties() const override;
    bool is_related_with_another(const Free_Space &other) const;
    bool is_inside_another(const Free_Space &other) const;
    bool is_adjacent_to_another_in_x(const Free_Space &other);
    bool is_adjacent_to_another_in_z(const Free_Space &other);
};



#endif //MASTERS_WORK_FREE_SPACE_H
