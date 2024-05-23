set (lib_name Block_Elements_Numbers)

add_library(${lib_name} ${lib_name}.h ${lib_name}.cpp)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})