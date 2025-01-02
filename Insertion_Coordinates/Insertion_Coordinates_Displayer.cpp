

#include "Insertion_Coordinates_Displayer.h"
#include <Box_Insertion_Coordinates.h>
#include <Block_Insertion_Coordinates.h>
#include <list>


#include <string>

using std::to_string, std::string;

string make_tabs(unsigned int number) {
    string tabs;
    for (unsigned int i = 0; i < number; i++) {
        tabs += '\t';
    }
    return tabs;
}

string Insertion_Coordinates_Displayer::display_all_coordinates_in_list(std::list<std::unique_ptr<A_Insertion_Coordinates>> &list) {
    std::string json="{\n";
    number_of_tabs=1;
    for(const auto &element : list) {
        json+=element->accept(this);
    }
    json+="}\n";
    number_of_tabs=0;
    return json;
}

string Insertion_Coordinates_Displayer::display_block_coordinates(Block_Insertion_Coordinates *coordinates) {
    string json=make_tabs(number_of_tabs)+"\"Block\":{\n";
    json+=make_tabs(number_of_tabs)+"\t\"id\":"+to_string(coordinates->get_element_id())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\"start point\":{\n";
    json+=make_tabs(number_of_tabs)+"\t\t\"x\":"+to_string(coordinates->get_start_point().get_x())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\t\"z\":"+to_string(coordinates->get_start_point().get_z())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\t\"y\":"+to_string(coordinates->get_start_point().get_y())+",\n";
    json+=make_tabs(number_of_tabs)+"\t},\n";
    json+=make_tabs(number_of_tabs)+"\t\"Elements\":["+"\n";
    for(auto &sub_element : coordinates->get_sub_elements()) {
        number_of_tabs++;
        json+=sub_element->accept(this);
        number_of_tabs--;
    }
    json+=make_tabs(number_of_tabs)+"\t"+make_tabs(number_of_tabs)+"]\n";
    json+=make_tabs(number_of_tabs)+"\twidth:"+to_string(coordinates->get_sizes().get_width())+",\n";
    json+=make_tabs(number_of_tabs)+"\tdepth:"+to_string(coordinates->get_sizes().get_depth())+",\n";
    json+=make_tabs(number_of_tabs)+"\theight:"+to_string(coordinates->get_sizes().get_height())+"\n";
    return json+make_tabs(number_of_tabs)+"}\n";
}

string Insertion_Coordinates_Displayer::display_box_coordinates(Box_Insertion_Coordinates *coordinates) const {
    return make_tabs(number_of_tabs)+"\"Box\":{\n"+
                make_tabs(number_of_tabs)+"\tid:"+to_string(coordinates->get_element_id())+",\n"+
                make_tabs(number_of_tabs)+"\tstart point:{\n"+
                    make_tabs(number_of_tabs)+"\t\tx:"+to_string(coordinates->get_start_point().get_x())+",\n"+
                    make_tabs(number_of_tabs)+"\t\tz:"+to_string(coordinates->get_start_point().get_z())+",\n"+
                    make_tabs(number_of_tabs)+"\t\ty:"+to_string(coordinates->get_start_point().get_y())+",\n"+
                make_tabs(number_of_tabs)+"\t},\n"+
                make_tabs(number_of_tabs)+"\twidth:"+to_string(coordinates->get_sizes().get_width())+",\n"+
                make_tabs(number_of_tabs)+"\tdepth:"+to_string(coordinates->get_sizes().get_depth())+",\n"+
                make_tabs(number_of_tabs)+"\theight:"+to_string(coordinates->get_sizes().get_height())+"\n"+
            make_tabs(number_of_tabs)+"}\n";
}
