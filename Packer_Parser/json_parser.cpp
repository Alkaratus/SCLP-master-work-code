#include <algorithm>
#include <fstream>
#include <cassert>
#include <cctype>

#include "json_parser.h"

#include <map>


using namespace JsonParser;

std::any JsonParser::parse_json(const std::string& filepath) {
  std::string text;
  read_file(filepath, text);

  // 2. parse the text with the helper function and return
  text_it start = text.begin();
  return parse_json_helper(text, start);
}

void JsonParser::read_file(const std::string& filepath, std::string& output) {
  std::ifstream file(filepath);
  std::string line;

  while (std::getline(file, line)) {
    output.append(line); // append() copies the argument passed as a reference(&std::string)
  }
}

std::any JsonParser::parse_primitive(const std::string& text, text_it start, text_it end) {
  std::string substr = text.substr(start - text.begin(), end - start);
  size_t float_point_index = substr.find('.');
  if (float_point_index >= (end - start)) { // integer
    return std::stoll(substr);
  }
  return std::stod(substr);
}

std::pair<std::string, std::any> JsonParser::retrive_key_value_pair(
  const std::string& text,
  text_it& it
) {
  assert(it != text.end());

  // ignore white spaces & line breaks
  while (*it == ' ' || *it == '\n') {
    it++;
  }

  text_it curr_it;
  std::string key;
  // if hit a double quote for the first time, it is a key
  if (*it == '\"') {
    curr_it = ++it;
    while (*it != '\"') {
      it++;
    }

    key = text.substr(curr_it - text.begin(), it - curr_it);
    assert(*(++it) == ':'); // assert that we are parsing the key string
    it++;
  }

  // now we need to have its corresponding value
  while (*it == ' ' || *it == '\n') {
    it++;
  }

  if (*it == '{') {
    return std::make_pair(key, parse_json_helper(text, it));
  }
  if(*it == '[') {
    return std::make_pair(key,parse_json_list(text, it));
  }
  if (*it == '"') {

  }
  else {
    // primitive value(double or int)
    curr_it = it;
    while (isdigit(*it) || *it == '.') {
      it++;
    }
     return std::make_pair(key,parse_primitive(text, curr_it, it));
  }
}

std::any JsonParser::parse_json_helper(const std::string& text, text_it& it) {
  assert(*it == '{'); // must start with the left curly bracket
  it++;

  std::map<std::string, std::any> json_map;

  do {
    auto [key, value] = retrive_key_value_pair(text, it);
    json_map[key] = std::move(value);
    if (*it == ',') {
      it++;
    }
    while (*it == ' ' || *it == '\n') {
      it++;
    }
  } while (*it != '}');

  it++; // after '}'
  return json_map;
}

std::any JsonParser::parse_json_list(const std::string &text, text_it &it) {
  assert(*it == '['); // must start with the left curly bracket
  it++;
  while (*it == ' ' || *it == '\n') {
    it++;
  }
  std::list<std::any> list;

  do {
    list.emplace_back(parse_json_helper(text, it));
    if (*it == ',') {
      it++;
    }
    while (*it == ' ' || *it == '\n') {
      it++;
    }
  } while (*it != ']');
  it++; // after ']'
  return list;
}
