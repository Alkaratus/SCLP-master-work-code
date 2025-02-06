
#ifndef INSERTION_COORDINATES_DISPLAYER_H
#define INSERTION_COORDINATES_DISPLAYER_H
#include <list>
#include <memory>
#include <string>

#include "I_Coordinates_Displayer.h"
#include "Point_3D.h"



class Insertion_Coordinates_Displayer: public I_Coordinates_Displayer {
    unsigned int number_of_tabs=0;
protected:
    [[nodiscard]] std::string get_tabs() const;
    [[nodiscard]] std::string display_start_point_coordinates(const Point_3D &start_point) const;

public:
    std::string display_all_coordinates_in_list(std::list<std::unique_ptr<A_Insertion_Coordinates>>& list);
    std::string display_block_coordinates(Block_Insertion_Coordinates *coordinates) override;
    std::string display_box_coordinates(Box_Insertion_Coordinates *coordinates) override;
};



#endif //INSERTION_COORDINATES_DISPLAYER_H
