set (lib_name Insertable_Element)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_link_libraries(${lib_name} PUBLIC Element_3D)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})