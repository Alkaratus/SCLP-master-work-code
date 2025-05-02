//
// Created by kubam on 02.03.2024.
//

#ifndef MASTERS_WORK_FREE_SPACE_H
#define MASTERS_WORK_FREE_SPACE_H

#include "Point_3D.h"
#include "Insertable_Element.h"

class Free_Space:public Element_3D {
    static unsigned int next_id;
    unsigned int id;
    Point_3D start_point;

    [[nodiscard]] bool share_x_zone_with(const Free_Space &other) const;
    [[nodiscard]] bool share_z_zone_with(const Free_Space &other) const;

public:
    Free_Space(const Point_3D &start, unsigned int width, unsigned int depth, unsigned int height);

    [[nodiscard]] std::string get_properties() const override;

    [[nodiscard]] Point_3D get_start_corner() const;
    [[nodiscard]] Point_3D get_wider_corner() const;
    [[nodiscard]] Point_3D get_deeper_corner() const;
    [[nodiscard]] Point_3D get_end_corner() const;
    [[nodiscard]] unsigned int get_id() const;

    [[nodiscard]] bool can_element_be_inserted(const Insertable_Element* element) const;

    [[nodiscard]] bool is_point_deep_inside(const Point_2D &point) const;
    [[nodiscard]] bool is_point_inside(const Point_2D &point) const;

    [[nodiscard]] bool is_related_with_another(const Free_Space &other) const;
    [[nodiscard]] bool is_inside_another(const Free_Space &other) const;
    [[nodiscard]] bool is_adjacent_to_another_in_x(const Free_Space &other) const;
    [[nodiscard]] bool is_adjacent_to_another_in_z(const Free_Space &other) const;
};



#endif //MASTERS_WORK_FREE_SPACE_H
