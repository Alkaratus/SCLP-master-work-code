set (lib_name Block)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_include_directories(${lib_name} PUBLIC .)