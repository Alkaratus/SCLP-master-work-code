//
// Created by kubam on 17.03.2024.
//

#ifndef MASTERS_WORK_A_INSERTION_COORDINATES_H
#define MASTERS_WORK_A_INSERTION_COORDINATES_H


#include "Insertable_Element.h"
#include "Point_3D.h"
#include "I_Coordinates_Displayer.h"

class A_Insertion_Coordinates {
    Point_3D start_point;
    Element_3D sizes;

public:
    virtual ~A_Insertion_Coordinates() = default;

    A_Insertion_Coordinates(Point_3D start_point,Element_3D sizes);

    [[nodiscard]] Point_3D get_start_point() const;
    [[nodiscard]] Point_3D get_wider_point() const;
    [[nodiscard]] Point_3D get_deeper_point() const;
    [[nodiscard]] Point_3D get_end_point() const;

    [[nodiscard]] Element_3D get_sizes() const;
    virtual std::string accept(I_Coordinates_Displayer* displayer)=0;

};


#endif //MASTERS_WORK_A_INSERTION_COORDINATES_H
