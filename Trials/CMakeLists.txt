set (lib_name Trials)

add_executable(${lib_name}
        main.cpp
        ${lib_name}.h
        ${lib_name}.cpp
)


target_link_libraries(${lib_name} PUBLIC Packer)

target_include_directories(${lib_name} PUBLIC ${CMAKE_CURRENT_LIST_DIR})