#ifndef VISUALISATION_COORDINATES_DISPLAYER_H
#define VISUALISATION_COORDINATES_DISPLAYER_H
#include <map>

#include "Insertion_Coordinates_Displayer.h"
#include "../Packer/A_Packer.h"

class Element_3D_Comparer:public std::less<Element_3D> {
    public:
    constexpr bool operator()(const Element_3D& lhs, const Element_3D& rhs) const
    {
        return compare_3D_elements_lexically(lhs,rhs); // assumes that the implementation handles pointer total order
    }
};

class Visualisation_Coordinates_Displayer: public I_Coordinates_Displayer {
    unsigned int new_element_id=1;
    unsigned int box_id=1;
    unsigned int number_of_tabs=0;
    std::list<unsigned int> box_types_ids;
    std::map<Element_3D, unsigned int,Element_3D_Comparer> SKU_ids;

    std::string add_container_size(const Container& container);
    std::string add_box_types(const std::list<Elements_Group>& groups);
    std::string add_inserted_elements_data(std::list<std::unique_ptr<A_Insertion_Coordinates>>& list);
    std::string add_locations(std::list<std::unique_ptr<A_Insertion_Coordinates>>& list);
    public:
    std::string get_data_for_visualisation(A_Packer *packer, std::list<std::unique_ptr<A_Insertion_Coordinates>>& list);
    std::string display_block_coordinates(Block_Insertion_Coordinates *coordinates) override;
    std::string display_box_coordinates(Box_Insertion_Coordinates *coordinates) override;
};



#endif //VISUALISATION_COORDINATES_DISPLAYER_H
