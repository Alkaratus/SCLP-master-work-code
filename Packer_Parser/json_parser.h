#ifndef JSON_PARSER_H
#define JSON_PARSER_H

#include <any>
#include <list>
#include <map>
#include <string>
#include <memory>
#include <utility> // for std::pair

namespace JsonParser {
    using text_it = std::string::iterator;
    using integer= long long int;
    using real= double;
    using text= std::string;
    using json_objects_list = std::list<std::any>;
    using json_object = std::map<std::string, std::any>;
    /**
    A union value representing the value part of (key, value) pair.
    */

    void read_file(const std::string& filepath, std::string& output);

    std::any parse_primitive(const std::string& text, text_it start, text_it end);

    std::pair<std::string, std::any> retrive_key_value_pair(const std::string& text,text_it& it);

    std::any parse_json_helper(const std::string& text, text_it& it);

    std::any parse_json_list(const std::string& text, text_it& it);

    std::any parse_json(const std::string& filepath);
}

#endif
