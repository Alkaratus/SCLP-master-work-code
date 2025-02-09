set (target main)

add_executable(${target} main.cpp)

target_link_libraries(${target} PUBLIC Box_Generators Container Elements_Group Packer Packer_Parser)