cmake_minimum_required(VERSION 3.29)
project(Flamanville C)

set(CMAKE_C_STANDARD 11)

include_directories(src)

add_executable(Flamanville
        src/constantes.h
        src/dessin.c
        src/dessin.h
        src/file_manager.c
        src/file_manager.h
        src/main.c
        src/structures.h
        src/transformations.c
        src/transformations.h)

add_custom_command(
        TARGET Flamanville POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets $<TARGET_FILE_DIR:Flamanville>/assets
)