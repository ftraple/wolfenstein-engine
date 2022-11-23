cmake_minimum_required(VERSION 3.10)

option(BUILD_MYLIBRARY "Build w3d library" ON)
option(BUILD_TESTS "Build the test application." On)

if(BUILD_MYLIBRARY)
    add_subdirectory(w3d)
endif()

if(BUILD_TESTS)
    add_subdirectory(test)
endif()