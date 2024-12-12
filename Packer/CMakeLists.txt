set (lib_name Packer)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp
        A_Packer.cpp
        A_Packer.h)

target_link_libraries(${lib_name} PUBLIC Container Visitor Box Elements_Group Simple_Block Complex_Block)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})