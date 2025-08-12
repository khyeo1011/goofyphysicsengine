cmake_minimum_required(VERSION 3.16)

project(goofyphysicsengine VERSION 0.1 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(GLAD_GL "${CMAKE_CURRENT_LIST_DIR}/src/gl.c" )
set(MAIN_SOURCE_DIR "${CMAKE_CURRENT_LIST_DIR}/src/main")

include(FetchContent)
FetchContent_Declare(
  glfw
  GIT_REPOSITORY https://github.com/glfw/glfw.git
  GIT_TAG        7b6aead9fb88b3623e3b3725ebb42670cbe4c579 # release-3.4.0
)

FetchContent_MakeAvailable(glfw)
include_directories(include)
include_directories(deps)
link_libraries(glfw)
add_executable(${PROJECT_NAME} 
${MAIN_SOURCE_DIR}/main.cpp
${GLAD_GL})

add_executable(heightmap
  src/demo/heightmap.cpp
  ${GLAD_GL})


target_include_directories(${PROJECT_NAME} PRIVATE src)


