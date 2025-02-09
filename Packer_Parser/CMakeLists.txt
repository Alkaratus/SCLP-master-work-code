set (lib_name Packer_Parser)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp
json_parser.h json_parser.cpp)

target_link_libraries(${lib_name} PUBLIC Packer)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})