//
// Created by kubam on 24.01.2025.
//

#ifndef I_COORDINATES_DISPLAYER_H
#define I_COORDINATES_DISPLAYER_H
#include <string>

class A_Insertion_Coordinates;
class Box_Insertion_Coordinates;
class Block_Insertion_Coordinates;

class I_Coordinates_Displayer {
public:
    virtual ~I_Coordinates_Displayer()=default;

    virtual std::string display_block_coordinates(Block_Insertion_Coordinates *coordinates)=0;
    virtual std::string display_box_coordinates(Box_Insertion_Coordinates *coordinates)=0;
};



#endif //I_COORDINATES_DISPLAYER_H
