//
// Created by kubam on 18.09.2024.
//

#include "Box_Insertion_Coordinates.h"

Box_Insertion_Coordinates::Box_Insertion_Coordinates(Point_3D start, Box *box):A_Insertion_Coordinates(start,Element_3D(box->get_width(),box->get_depth(),box->get_height())),element_id(box->get_id()) {
}

std::string Box_Insertion_Coordinates::create_json() {
    return "box:{\n"
                "id:"+std::to_string(element_id)+",\n"+
                "start point:{\n"
                    "x:"+std::to_string(get_start_point().get_x())+",\n"+
                    "z:"+std::to_string(get_start_point().get_z())+",\n"+
                    "y:"+std::to_string(get_start_point().get_y())+",\n"+
                "},\n"
                "width:"+std::to_string(get_sizes().get_width())+",\n"+
                "depth:"+std::to_string(get_sizes().get_depth())+",\n"+
                "height:"+std::to_string(get_sizes().get_height())+",\n"
            "}";
}
