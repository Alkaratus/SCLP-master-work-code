cmake_minimum_required(VERSION 3.08)

project(masters_work)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(main)
add_subdirectory(Trials)

add_subdirectory(Blocks)
add_subdirectory(Block_Elements_Numbers)
add_subdirectory(Box)
add_subdirectory(Box_Generators)
add_subdirectory(Container)
add_subdirectory(Element_3D)
add_subdirectory(Elements_Group)
add_subdirectory(Free_Spaces)
add_subdirectory(Insertable_Element)
add_subdirectory(Insertion_Coordinates)
add_subdirectory(Packer)
add_subdirectory(Packer_Parser)
add_subdirectory(Points)
add_subdirectory(Visitor)


enable_testing()
add_subdirectory(Tests)