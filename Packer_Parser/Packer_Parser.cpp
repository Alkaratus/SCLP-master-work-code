//
// Created by kubam on 26.01.2025.
//

#include "Packer_Parser.h"
#include "json_parser.h"

Packer generate_packer(const std::string& filepath) {
    auto extracted=JsonParser::parse_json(filepath);
    auto object=std::any_cast<JsonParser::json_object>(extracted);
    auto json_container=std::any_cast<JsonParser::json_object>(object["container"]);

    auto json_boxes=std::any_cast<JsonParser::json_objects_list>(object["boxes"]);
    std::list<Box> boxes;
    for (auto json_box : json_boxes) {
        auto casted=std::any_cast<JsonParser::json_object>(json_box);
        boxes.emplace_back(std::any_cast<long long int>(casted["width"]),std::any_cast<long long int>(casted["depth"]),std::any_cast<long long int>(casted["height"]));
    }

    return {boxes,Container(std::any_cast<long long int>(json_container["width"]),
        std::any_cast<long long int>(json_container["depth"]),std::any_cast<long long int>(json_container["height"]))};
}
