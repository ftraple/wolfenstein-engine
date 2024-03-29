cmake_minimum_required(VERSION 3.10)

project(w3d VERSION 1.0.0)

# Add the GNU install directories
include(GNUInstallDirs)

# specify the C++ standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Set the library source files
set(SOURCE_FILES 
    source/Camera.c
    source/Map.c
    source/Sprite.c
    source/SpriteList.c
    source/Texture.c
    source/TextureList.c
)

# Generate the library
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
    PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}>)

# Generate a library with version number and link
set_target_properties(${PROJECT_NAME} PROPERTIES
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# Share the version with the library code trough 
target_compile_definitions(${PROJECT_NAME} PRIVATE
    FOO_MAJOR_VERSION=${PROJECT_VERSION_MAJOR}
    FOO_MINOR_VERSION=${PROJECT_VERSION_MINOR}
)

install(DIRECTORY "include/"
    # Must match the INSTALL_INTERFACE of include directories, see above.
    DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/${PROJECT_NAME}"
)

# Instalation
install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}_Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)