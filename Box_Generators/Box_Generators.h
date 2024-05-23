//
// Created by kubam on 04.03.2024.
//

#ifndef MASTERS_WORK_BOX_GENERATORS_H
#define MASTERS_WORK_BOX_GENERATORS_H

#include <list>
#include "Box.h"

std::list<Box>generate_boxes(unsigned int volume, unsigned int method());
std::list<Box>generate_boxes(unsigned int volume,unsigned int method(unsigned int), unsigned int max_size);

Box generate_box(unsigned int method());
Box generate_box(unsigned int method(unsigned int), unsigned int max_value);

unsigned int geometrical_distribution();
unsigned int geometrical_distribution(unsigned int max_value);

#endif //MASTERS_WORK_BOX_GENERATORS_H
