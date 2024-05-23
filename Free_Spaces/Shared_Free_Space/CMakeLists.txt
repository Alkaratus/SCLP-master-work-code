set (lib_name Shared_Free_Space)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_link_libraries(${lib_name} PUBLIC Point_2D)

target_include_directories(${lib_name} PUBLIC .)