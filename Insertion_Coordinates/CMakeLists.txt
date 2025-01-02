set (lib_name Insertion_Coordinates)

add_library(${lib_name}
        A_Insertion_Coordinates.h A_Insertion_Coordinates.cpp
        Box_Insertion_Coordinates.h Box_Insertion_Coordinates.cpp
        Block_Insertion_Coordinates.h Block_Insertion_Coordinates.cpp
        Insertion_Coordinates_Factory.h Insertion_Coordinates_Factory.cpp
        Insertion_Coordinates_Displayer.h
        Insertion_Coordinates_Displayer.cpp
)

target_link_libraries(${lib_name} PUBLIC Insertable_Element Point_3D Block Box Visitor)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})