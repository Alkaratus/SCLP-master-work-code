set (lib_name Container)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp ${lib_name}_Free_Space.cpp
        ${lib_name}_Anchored_Free_Space.cpp)

target_link_libraries(${lib_name} PUBLIC Element_3D Free_Space Insertable_Element Insertion_Coordinates)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})