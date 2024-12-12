//
// Created by kubam on 11.03.2024.
//

#include "Block.h"

Block::Block(unsigned int width, unsigned int depth, unsigned int height) : Insertable_Element(width, depth, height) {

}

Block::Block(const Block &other):Insertable_Element(other) {
}
