cmake_minimum_required(VERSION 3.14)
project(DiceProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# FetchContent module is available from CMake 3.11 onwards
include(FetchContent)

# Download and configure GLFW
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        3.4  # You can specify the version or branch you want
)

# Download and make glfw available
FetchContent_MakeAvailable(glfw)

# Add your source files
add_executable(DiceProject src/main.cpp)

# Link GLFW to your project
target_link_libraries(DiceProject PRIVATE glfw)

# Include the GLFW directories to ensure headers are found
target_include_directories(DiceProject PRIVATE ${glfw_SOURCE_DIR}/include)

# For Linux and MacOS, you might need to link additional libraries
if(UNIX)
  target_link_libraries(DiceProject PRIVATE pthread dl)
endif()

# For MacOS, link the necessary frameworks
if(APPLE)
  find_library(OpenGL_LIBRARY OpenGL)
  target_link_libraries(DiceProject PRIVATE
    ${OpenGL_LIBRARY}
    "-framework Cocoa"
    "-framework IOKit"
    "-framework CoreVideo"
  )
endif()
