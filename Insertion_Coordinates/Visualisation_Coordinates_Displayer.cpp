//
// Created by kubam on 03.01.2025.
//
#include <string>

#include "Block_Insertion_Coordinates.h"
#include "Box_Insertion_Coordinates.h"

using std::string, std::to_string;

#include "Visualisation_Coordinates_Displayer.h"

string Visualisation_Coordinates_Displayer::add_container_size(const Container& container) {
    string data=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeX":)"+to_string(container.get_width())+",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeZ":)"+to_string(container.get_depth())+",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeY":)"+to_string(container.get_height())+",\n";
    return data;
}

string Visualisation_Coordinates_Displayer::add_box_types(const std::list<Elements_Group> &groups) {
    string data=string(number_of_tabs,'\t')+R"("SKUList":{)"+'\n';
    number_of_tabs++;
    data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
    unsigned int SKU_id=1;
    for(auto group=groups.cbegin(); group!=groups.cend(); ++group) {
        SKU_ids.emplace(group->get_group_element_properties(),SKU_id);
        data+=string(number_of_tabs,'\t')+'\"'+to_string(SKU_id)+"\":{\n";
        number_of_tabs++;
        data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
        data+=string(number_of_tabs,'\t')+R"("Id":")"+to_string(SKU_id++)+"\",\n";
        data+=string(number_of_tabs,'\t')+R"("SizeX":)"+to_string(group->get_group_element_properties().get_width())+",\n";
        data+=string(number_of_tabs,'\t')+R"("SizeY":)"+to_string(group->get_group_element_properties().get_height())+",\n";
        data+=string(number_of_tabs,'\t')+R"("SizeZ":)"+to_string(group->get_group_element_properties().get_depth())+",\n";
        data+=string(number_of_tabs,'\t')+R"("Weight": 0,)"+'\n';
        data+=string(number_of_tabs,'\t')+R"("LoadCapacity": 0,)"+'\n';
        data+=string(number_of_tabs,'\t')+R"("MaxOverhang": 0,)"+'\n';
        data+=string(number_of_tabs,'\t')+R"("MaxOverhangX": 0,)"+'\n';
        data+=string(number_of_tabs,'\t')+R"("MaxOverhangY": 0,)"+'\n';
        data+=string(number_of_tabs,'\t')+R"("FragileTop": false,)"+'\n';
        data+=string(number_of_tabs,'\t')+R"("UnevenBottom": false)"+'\n';
        number_of_tabs--;
        data+=string(number_of_tabs,'\t')+"}";
        data+=group==(--groups.end())?"\n":",\n";
    }
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"},";
    return data;
}

string Visualisation_Coordinates_Displayer::add_inserted_elements_data(std::list<std::unique_ptr<A_Insertion_Coordinates>> &list) {
    string data=string(number_of_tabs,'\t')+R"("BoxTypes":[)"+'\n';
    number_of_tabs++;
    for (auto it=list.begin(); it!=list.end(); ++it) {
        data+=(*it)->accept(this);
        data+=it==(--list.end())?"\n":",\n";
    }
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"],";
    return data;
}

string Visualisation_Coordinates_Displayer::add_locations(std::list<std::unique_ptr<A_Insertion_Coordinates>> &list) {
    string data=string(number_of_tabs,'\t')+R"("Pallets":[)"+'\n';
    number_of_tabs++;
    data+=string(number_of_tabs,'\t')+"{\n";
    number_of_tabs++;
    data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("BoxPos":[)"+'\n';
    number_of_tabs++;
    auto id=box_types_ids.begin();
    for (auto it=list.begin(); it!=list.end(); ++it) {
        data+=string(number_of_tabs,'\t')+"{\n";
        number_of_tabs++;
        data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
        data+=string(number_of_tabs,'\t')+R"("Item1":{)"+"\n";
        number_of_tabs++;
        data+=string(number_of_tabs,'\t')+R"("$ref":")"+to_string(*id)+"\"\n";
        ++id;
        number_of_tabs--;
        data+=string(number_of_tabs,'\t')+"},\n";
        data+=string(number_of_tabs,'\t')+R"("Item2":{)"+"\n";
        number_of_tabs++;
        data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
        data+=string(number_of_tabs,'\t')+R"("X":)"+to_string((*it)->get_start_point().get_x())+",\n";
        data+=string(number_of_tabs,'\t')+R"("Y":)"+to_string((*it)->get_start_point().get_y())+",\n";
        data+=string(number_of_tabs,'\t')+R"("Z":)"+to_string((*it)->get_start_point().get_z())+",\n";
        data+=string(number_of_tabs,'\t')+R"("Rotated": false)"+'\n';
        number_of_tabs--;
        data+=string(number_of_tabs,'\t')+"}\n";
        number_of_tabs--;
        data+=string(number_of_tabs,'\t')+"}";
        box_id+=3;
        data+=string(number_of_tabs,'\t')+(it==(--list.end())?"\n":",\n");
    }
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"]\n";
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"}\n";
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"]\n";
    return data;
}

string Visualisation_Coordinates_Displayer::get_data_for_visualisation(A_Packer *packer,std::list<std::unique_ptr<A_Insertion_Coordinates>> &list) {
    new_element_id=1;
    std::string data="{\n";
    number_of_tabs++;
    data+=add_container_size(packer->get_container_data());
    data+=add_box_types(packer->get_elements_groups_data());
    data+=add_inserted_elements_data(list);
    data+=add_locations(list);
    number_of_tabs--;
    return data+"\n}";
}

std::string Visualisation_Coordinates_Displayer::display_block_coordinates(Block_Insertion_Coordinates *coordinates) {
    string data=string(number_of_tabs,'\t')+"{\n";
    number_of_tabs++;
    box_types_ids.emplace_back(new_element_id);
    data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("Id":"BOX)"+to_string(box_id++)+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeX":)"+to_string(coordinates->get_sizes().get_width())+",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeY":)"+to_string(coordinates->get_sizes().get_height())+",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeZ":)"+to_string(coordinates->get_sizes().get_depth())+",\n";
    data+=string(number_of_tabs,'\t')+R"("Weight": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("LoadCapacity": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("MaxOverhang": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("MaxOverhangX": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("MaxOverhangY": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("FragileTop": false,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("UnevenBottom": false,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("Num": )"+to_string(coordinates->get_sub_elements().size())+",\n";
    data+=string(number_of_tabs,'\t')+R"("Items":[)"+'\n';
    number_of_tabs++;
    for(auto it=coordinates->get_sub_elements().begin();it!=coordinates->get_sub_elements().end();++it) {
        data+=string(number_of_tabs,'\t')+"{\n";
        number_of_tabs++;
        data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
        auto sizes=(*it)->get_sizes();
        data+=string(number_of_tabs,'\t')+R"("Item1":")"+std::to_string(SKU_ids.at(sizes))+"\",\n";
        data+=string(number_of_tabs,'\t')+R"("Item2":{)"+"\n";
        number_of_tabs++;
        data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
        data+=string(number_of_tabs,'\t')+R"("X":)"+to_string((*it)->get_start_point().get_x())+",\n";
        data+=string(number_of_tabs,'\t')+R"("Y":)"+to_string((*it)->get_start_point().get_y())+",\n";
        data+=string(number_of_tabs,'\t')+R"("Z":)"+to_string((*it)->get_start_point().get_z())+",\n";
        data+=string(number_of_tabs,'\t')+R"("Rotated": false)"+'\n';
        number_of_tabs--;
        data+=string(number_of_tabs,'\t')+"}\n";
        number_of_tabs--;
        data+=string(number_of_tabs,'\t')+"}";
        data+=it==(--coordinates->get_sub_elements().end())?"\n":",\n";
    }
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+R"(])"+'\n';
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"}";
    return data;
}

std::string Visualisation_Coordinates_Displayer::display_box_coordinates(Box_Insertion_Coordinates *coordinates){
    string data=string(number_of_tabs,'\t')+"{\n";
    number_of_tabs++;
    box_types_ids.emplace_back(new_element_id);
    data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("Id":"BOX)"+to_string(box_id++)+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeX":)"+to_string(coordinates->get_sizes().get_width())+",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeY":)"+to_string(coordinates->get_sizes().get_height())+",\n";
    data+=string(number_of_tabs,'\t')+R"("SizeZ":)"+to_string(coordinates->get_sizes().get_depth())+",\n";
    data+=string(number_of_tabs,'\t')+R"("Weight": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("LoadCapacity": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("MaxOverhang": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("MaxOverhangX": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("MaxOverhangY": 0,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("FragileTop": false,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("UnevenBottom": false,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("Num": 1,)"+'\n';
    data+=string(number_of_tabs,'\t')+R"("Items":[)"+'\n';
    number_of_tabs++;
    data+=string(number_of_tabs,'\t')+"{\n";
    number_of_tabs++;
    data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
    auto sizes=coordinates->get_sizes();
    data+=string(number_of_tabs,'\t')+R"("Item1":")"+std::to_string(SKU_ids.at(sizes))+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("Item2":{)"+"\n";
    number_of_tabs++;
    data+=string(number_of_tabs,'\t')+R"("$id":")"+to_string(new_element_id++)+"\",\n";
    data+=string(number_of_tabs,'\t')+R"("X":0,)"+"\n";
    data+=string(number_of_tabs,'\t')+R"("Y":0,)"+"\n";
    data+=string(number_of_tabs,'\t')+R"("Z":0,)"+"\n";
    data+=string(number_of_tabs,'\t')+R"("Rotated": false)"+'\n';
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"}\n";
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"}\n";
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+R"(])"+'\n';
    number_of_tabs--;
    data+=string(number_of_tabs,'\t')+"}";
    return data;
}
