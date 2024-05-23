set (lib_name Complex_Block)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_link_libraries(${lib_name} PUBLIC Elements_Group)

target_include_directories(${lib_name} PUBLIC .)