//
// Created by kubam on 11.03.2024.
//

#include "Block.h"

Block::Block(const unsigned int width, const unsigned int depth, const unsigned int height) : Insertable_Element(width, depth, height) {

}

Block::Block(const Block &other):Insertable_Element(other) {
}
