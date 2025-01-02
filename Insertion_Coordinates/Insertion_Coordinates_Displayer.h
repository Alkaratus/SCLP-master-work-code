
#ifndef INSERTION_COORDINATES_DISPLAYER_H
#define INSERTION_COORDINATES_DISPLAYER_H
#include <list>
#include <memory>
#include <string>

class A_Insertion_Coordinates;
class Box_Insertion_Coordinates;
class Block_Insertion_Coordinates;

class Insertion_Coordinates_Displayer {
    unsigned int number_of_tabs=0;
public:
    std::string display_all_coordinates_in_list(std::list<std::unique_ptr<A_Insertion_Coordinates>>& list);
    std::string display_block_coordinates(Block_Insertion_Coordinates *coordinates);
    std::string display_box_coordinates(Box_Insertion_Coordinates *coordinates) const;
};



#endif //INSERTION_COORDINATES_DISPLAYER_H
