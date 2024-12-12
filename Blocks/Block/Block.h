//
// Created by kubam on 11.03.2024.
//

#ifndef MASTERS_WORK_BLOCK_H
#define MASTERS_WORK_BLOCK_H

#include "Insertable_Element.h"

class Block: public Insertable_Element {
public:
    Block(unsigned int width, unsigned int depth, unsigned int height);
    Block(const Block& other);
};


#endif //MASTERS_WORK_BLOCK_H
