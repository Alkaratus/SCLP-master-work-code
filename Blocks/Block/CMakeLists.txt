set (lib_name Block)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_link_libraries(${lib_name} PUBLIC Insertable_Element)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})