cmake_minimum_required(VERSION 3.10)
project(OpenGLHelloWorld)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(
    ${CMAKE_SOURCE_DIR}/../../ext/glfw
    ${CMAKE_BINARY_DIR}/glfw_build
)

include_directories(${CMAKE_SOURCE_DIR}/../../ext/glfw/include)

find_package(OpenGL REQUIRED)

add_executable(OpenGLHelloWorld src/demos/redanim.cpp)

target_link_libraries(OpenGLHelloWorld PRIVATE glfw OpenGL::GL)
