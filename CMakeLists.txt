cmake_minimum_required(VERSION 3.10)

# Set the project name
project(OpenGLWindow)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add the executable
add_executable(OpenGLWindow  main.cpp lib/glad/glad.c lib/glad_wgl/glad_wgl.c)

# Find OpenGL package
find_package(OpenGL REQUIRED)

# Include directories for OpenGL and GLEW
include_directories(${OPENGL_INCLUDE_DIRS} lib)

# Link the executable with the necessary libraries
target_link_libraries(OpenGLWindow ${OPENGL_LIBRARIES} opengl32)