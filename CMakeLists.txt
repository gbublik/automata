cmake_minimum_required(VERSION 3.22)
project(automat C)

set(CMAKE_C_STANDARD 99)
find_package(glfw3 3.3 REQUIRED)
add_library(glad /usr/include/glad/glad.h /usr/include/glad/glad.c)
find_library( DL_LIBRARY libdl.so )

add_executable(automat main.c /usr/include/glad/glad.c)
target_link_libraries(automat ${DL_LIBRARY})
target_link_libraries( automat ${OPENGL_LIBRARIES} glfw)
target_link_libraries(automat ${ITK_LIBRARIES} m)