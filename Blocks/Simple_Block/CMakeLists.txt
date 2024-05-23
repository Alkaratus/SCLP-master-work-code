set (lib_name Simple_Block)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_link_libraries(${lib_name} PUBLIC Box Block_Elements_Numbers)

target_include_directories(${lib_name} PUBLIC .)