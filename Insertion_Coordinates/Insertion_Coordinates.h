//
// Created by kubam on 17.03.2024.
//

#ifndef MASTERS_WORK_INSERTION_COORDINATES_H
#define MASTERS_WORK_INSERTION_COORDINATES_H

#include <memory>
#include "Insertable_Element.h"
#include "Point_3D.h"

class Insertion_Coordinates {
    Insertable_Element* inserted_element;
    Point_3D start_point;
public:
    Insertion_Coordinates(Insertable_Element* inserted_element, Point_3D start_point);
    [[nodiscard]] Point_3D get_start_point() const;
    [[nodiscard]] Point_3D get_wider_point() const;
    [[nodiscard]] Point_3D get_deeper_point() const;
    [[nodiscard]] Point_3D get_end_point() const;
    [[nodiscard]] const Insertable_Element* get_inserted_element() const;
};

#endif //MASTERS_WORK_INSERTION_COORDINATES_H
