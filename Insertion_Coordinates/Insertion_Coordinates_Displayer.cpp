

#include "Insertion_Coordinates_Displayer.h"
#include <Box_Insertion_Coordinates.h>
#include <Block_Insertion_Coordinates.h>
#include <list>


#include <string>

using std::to_string, std::string;

string make_tabs(const unsigned int number) {
    string tabs;
    for (unsigned int i = 0; i < number; i++) {
        tabs += '\t';
    }
    return tabs;
}

std::string Insertion_Coordinates_Displayer::get_tabs() const {
    return make_tabs(number_of_tabs);
}

string Insertion_Coordinates_Displayer::display_start_point_coordinates(const Point_3D &start_point) const {
    string json=make_tabs(number_of_tabs)+"\t\"start_point\":{\n";
    json+=make_tabs(number_of_tabs)+"\t\t\"x\":"+to_string(start_point.get_x())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\t\"z\":"+to_string(start_point.get_z())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\t\"y\":"+to_string(start_point.get_y())+"\n";
    json+=make_tabs(number_of_tabs)+"\t}";
    return json;
}

string Insertion_Coordinates_Displayer::display_all_coordinates_in_list(std::list<std::unique_ptr<A_Insertion_Coordinates>> &list) {
    std::string json="{\n\t\"Coordinates\":[";
    number_of_tabs=1;
    for(auto it=list.begin(); it!=list.end(); ++it) {
        json+=(*it)->accept(this);
        if(auto it2=it;++it2==list.end()) {
            json+="\n";
        }
        else {
            json+=",\n";
        }
    }
    json+="]\n}\n";
    number_of_tabs=0;
    return json;
}

string Insertion_Coordinates_Displayer::display_block_coordinates(Block_Insertion_Coordinates *coordinates) {
    string json=make_tabs(number_of_tabs)+"{\n";
    json+=make_tabs(number_of_tabs)+"\t\"id\":"+to_string(coordinates->get_element_id())+",\n";
    json+=display_start_point_coordinates(coordinates->get_start_point())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\"Elements\":["+"\n";
    auto &sub_elements=coordinates->get_sub_elements();
    for(int i=0;i<sub_elements.size();i++) {
        number_of_tabs++;
        json+=sub_elements[i]->accept(this);
        json+=i<sub_elements.size()-1?",\n":"\n";
        number_of_tabs--;
    }
    json+=make_tabs(number_of_tabs)+"\t"+make_tabs(number_of_tabs)+"],\n";
    json+=make_tabs(number_of_tabs)+"\t\"width\":"+to_string(coordinates->get_sizes().get_width())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\"depth\":"+to_string(coordinates->get_sizes().get_depth())+",\n";
    json+=make_tabs(number_of_tabs)+"\t\"height\":"+to_string(coordinates->get_sizes().get_height())+"\n";
    return json+make_tabs(number_of_tabs)+"}";
}

string Insertion_Coordinates_Displayer::display_box_coordinates(Box_Insertion_Coordinates *coordinates){
    return make_tabs(number_of_tabs)+"{\n"+
                make_tabs(number_of_tabs)+"\t\"id\":"+to_string(coordinates->get_element_id())+",\n"+
                display_start_point_coordinates(coordinates->get_start_point())+",\n"+
                make_tabs(number_of_tabs)+"\t\"width\":"+to_string(coordinates->get_sizes().get_width())+",\n"+
                make_tabs(number_of_tabs)+"\t\"depth\":"+to_string(coordinates->get_sizes().get_depth())+",\n"+
                make_tabs(number_of_tabs)+"\t\"height\":"+to_string(coordinates->get_sizes().get_height())+"\n"+
            make_tabs(number_of_tabs)+"}";
}
