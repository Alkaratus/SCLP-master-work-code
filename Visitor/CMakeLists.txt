set (lib_name Visitor)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_link_libraries(${lib_name} PUBLIC Box Simple_Block Complex_Block)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})