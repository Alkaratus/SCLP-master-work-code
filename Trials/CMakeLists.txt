set (lib_name Trials)

add_library(${lib_name}-lib
        ${lib_name}.h
        ${lib_name}.cpp
)

add_executable(${lib_name}
        main.cpp

)


target_link_libraries(${lib_name}-lib PUBLIC Packer )
target_include_directories(${lib_name}-lib PUBLIC ${CMAKE_CURRENT_LIST_DIR})

target_link_libraries(${lib_name} PUBLIC ${lib_name}-lib)