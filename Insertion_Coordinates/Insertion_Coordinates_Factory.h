//
// Created by kubam on 21.09.2024.
//

#ifndef MASTERS_WORK_INSERTION_COORDINATES_FACTORY_H
#define MASTERS_WORK_INSERTION_COORDINATES_FACTORY_H


#include "Visitor.h"
#include "Insertable_Element.h"
#include "Point_3D.h"
#include "A_Insertion_Coordinates.h"

class Insertion_Coordinates_Factory: public Visitor {
    Point_3D start_point;
    std::unique_ptr<A_Insertion_Coordinates> created_insertion_coordinate;
public:
    void visit(Box *box) override;
    void visit(Simple_Block *block) override;
    void visit(Complex_Block *block) override;

    std::unique_ptr<A_Insertion_Coordinates> create_insertion_coordinate(Insertable_Element *element,Point_3D point);
};


#endif //MASTERS_WORK_INSERTION_COORDINATES_FACTORY_H
