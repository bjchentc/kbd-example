cmake_minimum_required(VERSION 3.13)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

project(kb_example)

target_sources(app
    PRIVATE
    src/main.c
    )

include_directories(app
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src)
