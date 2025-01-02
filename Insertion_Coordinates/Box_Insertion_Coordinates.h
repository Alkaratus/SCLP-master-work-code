
#ifndef MASTERS_WORK_BOX_INSERTION_COORDINATES_H
#define MASTERS_WORK_BOX_INSERTION_COORDINATES_H
#include "A_Insertion_Coordinates.h"
#include "Box.h"

class Box_Insertion_Coordinates:public A_Insertion_Coordinates {
    unsigned int element_id;
public:
    Box_Insertion_Coordinates(Point_3D start, Box *box);
    std::string accept(Insertion_Coordinates_Displayer* displayer) override;

    unsigned int get_element_id();
};


#endif //MASTERS_WORK_BOX_INSERTION_COORDINATES_H
