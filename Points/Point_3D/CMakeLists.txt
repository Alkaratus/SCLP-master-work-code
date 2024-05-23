set (lib_name Point_3D)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_link_libraries(${lib_name} PUBLIC Point_2D)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})