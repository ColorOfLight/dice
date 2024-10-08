cmake_minimum_required(VERSION 3.14)
project(DiceProject)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Set Common directories
set(COMMON_INCLUDE_DIR include/common)
file(GLOB COMMON_HEADERS "${COMMON_INCLUDE_DIR}/*.h")
file(GLOB COMMON_SOURCE_FILES src/common/*.cpp src/main.cpp)

# Set Opengl directories
set(OPENGL_INCLUDE_DIR include/opengl)
file(GLOB OPENGL_HEADERS "${OpenGL_INCLUDE_DIR}/*.h")
file(GLOB OPENGL_SOURCE_FILES src/opengl/*.cpp)

# Set assmip options
option(ASSIMP_BUILD_STATIC_LIB ON)
option(ASSIMP_BUILD_ZLIB ON)
option(ASSIMP_NO_EXPORT ON)
option(ASSIMP_BUILD_ALL_IMPORTERS_BY_DEFAULT "Disable all importers by default" OFF)
option(ASSIMP_BUILD_ALL_EXPORTERS_BY_DEFAULT "Disable all exporters by default" OFF)
option(ASSIMP_BUILD_TESTS "Build test suite" OFF)
option(ASSIMP_INSTALL OFF)

set(ASSIMP_BUILD_OBJ_IMPORTER ON)
set(ASSIMP_BUILD_ZLIB ON)

if(NOT DEFINED TARGET)
  message(FATAL_ERROR "TARGET is not defined. Please set the TARGET variable.")
endif()

if (${TARGET} STREQUAL "WEBGL_EMSCRIPTEN")
  if(NOT DEFINED ENV{EMSCRIPTEN_ROOT})
      find_path(EMSCRIPTEN_ROOT "emcc")
  else()
    set(EMSCRIPTEN_ROOT "$ENV{EMSCRIPTEN_ROOT}")
  endif()

  if(NOT EMSCRIPTEN_ROOT)
    if(NOT DEFINED ENV{EMSDK})
      message(FATAL_ERROR "The emcc compiler not found in PATH")
    endif()
    set(EMSCRIPTEN_ROOT "$ENV{EMSDK}/upstream/emscripten")
  endif()

  if(EXISTS "${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake")
    set(CMAKE_TOOLCHAIN_FILE "${EMSCRIPTEN_ROOT}/cmake/Modules/Platform/Emscripten.cmake")
  else()
    message(FATAL_ERROR "Emscripten.cmake toolchain file not found")
  endif()

  include_directories(${COMMON_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} include/emscripten)

  file(GLOB_RECURSE DICE_SOURCES ${COMMON_HEADERS} ${OPENGL_HEADERS} ${COMMON_SOURCE_FILES} ${OPENGL_SOURCE_FILES} src/emscripten/*.cpp include/emscripten/*.h)

  add_executable(DiceProject ${DICE_SOURCES})

  # Copy assets to the build directory
  add_custom_target(copy_assets ALL
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
  )

  set_target_properties(DiceProject PROPERTIES LINK_FLAGS "-sMIN_WEBGL_VERSION=2 -sMAX_WEBGL_VERSION=2 -sALLOW_MEMORY_GROWTH=1 -fexceptions")
  target_compile_definitions(DiceProject PRIVATE TARGET_EMSCRIPTEN)
  target_link_options(DiceProject PUBLIC --preload-file assets)

elseif(${TARGET} STREQUAL "OPENGL_GLAD_GLFW")
  include_directories(${COMMON_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR} include/glfw)

  file(GLOB_RECURSE DICE_SOURCES ${COMMON_HEADERS} ${OPENGL_HEADERS} ${COMMON_SOURCE_FILES} ${OPENGL_SOURCE_FILES} src/glfw/*.cpp include/glfw/*.h)

  add_executable(DiceProject ${DICE_SOURCES})

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

  # Copy assets to the build directory
  add_custom_target(copy_assets ALL
      COMMAND ${CMAKE_COMMAND} -E copy_directory
          ${CMAKE_SOURCE_DIR}/assets ${CMAKE_BINARY_DIR}/assets
  )

  target_compile_definitions(DiceProject PRIVATE TARGET_GLFW)
else()
  # Print error message
  message(FATAL_ERROR "Invalid target: ${TARGET}")
endif()

add_subdirectory(third-party/glm-1.0.1)
add_subdirectory(third-party/bullet3 EXCLUDE_FROM_ALL)
add_subdirectory(third-party/assimp)

target_link_libraries(DiceProject PRIVATE
  glm::glm BulletDynamics BulletCollision LinearMath assimp)

target_include_directories(DiceProject PRIVATE
    third-party/glm-1.0.1/glm
    third-party/bullet3/src
    third-party/assimp/include
)

