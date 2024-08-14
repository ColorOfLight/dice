cmake_minimum_required(VERSION 3.14)
project(DiceProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Add your source files
add_executable(DiceProject src/main.cpp)

# Add third-pary libraries

add_subdirectory(third-party/glad-4.3)
target_link_libraries(DiceProject PRIVATE glad)

add_subdirectory(third-party/glfw-3.4)
target_link_libraries(DiceProject PRIVATE glfw)

target_include_directories(DiceProject PRIVATE
    third-party/glad-4.3/include
    third-party/glfw-3.4/include
)

if (WIN32)
    target_link_libraries(DiceProject PRIVATE opengl32)
elseif(APPLE)
    find_library(COCOA_LIBRARY Cocoa)
    find_library(OpenGL_LIBRARY OpenGL)
    find_library(IOKit_LIBRARY IOKit)
    find_library(CoreVideo_LIBRARY CoreVideo)
    target_link_libraries(DiceProject PRIVATE ${COCOA_LIBRARY} ${OpenGL_LIBRARY} ${IOKit_LIBRARY} ${CoreVideo_LIBRARY})
elseif(UNIX)
    target_link_libraries(DiceProject PRIVATE GL X11 pthread Xrandr Xi dl)
endif()