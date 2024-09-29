

# Add the executable
add_library(GSPCore STATIC
    lib/GSPCore.cpp
    lib/GWindow/GWindow_Win32.cpp
)

find_package(OpenGL REQUIRED)

# Include directories for OpenGL and GLEW
target_include_directories(GSPCore PRIVATE ${OPENGL_INCLUDE_DIRS} lib)

# Link the executable with the necessary libraries
target_link_libraries(GSPCore ${OPENGL_LIBRARIES} opengl32)

