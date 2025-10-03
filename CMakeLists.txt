cmake_minimum_required(VERSION 3.16)

project(goofyphysicsengine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "GLFW lib only")
set(GLFW_INSTALL OFF CACHE BOOL "GLFW lib only")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin")
set(CMAKE_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")
set(GLAD_GL "${CMAKE_CURRENT_LIST_DIR}/src/gl.c")
set(UTIL_DIR "${PROJECT_SOURCE_DIR}/src/util")
set(MAIN_SOURCE_DIR2D "${CMAKE_CURRENT_LIST_DIR}/src/2d")

include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG 7b6aead9fb88b3623e3b3725ebb42670cbe4c579 # release-3.4.0
)

FetchContent_MakeAvailable(glfw)
include_directories(include)
include_directories(deps)
link_libraries(glfw)

add_executable(goofyphysicsengine2d src/2d/main.cpp src/2d/object.cpp src/2d/rectangle.cpp src/2d/2dsimulator.cpp src/2d/circle.cpp src/2d/triangle.cpp src/2d/collision.cpp src/util/util.cpp deps/getopt.c src/gl.c)



target_include_directories(${PROJECT_NAME}2d PRIVATE src)
