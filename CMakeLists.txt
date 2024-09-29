

# Add the executable
add_library(GSPCore STATIC
    lib/GSPCore.cpp
    lib/GWindow/GWindow.cpp
    lib/GWindow/Win32/Win32GWindow.cpp
)

find_package(OpenGL REQUIRED)

# Include directories for OpenGL and GLEW
target_include_directories(GSPCore PRIVATE ${OPENGL_INCLUDE_DIRS} lib)

# Link the executable with the necessary libraries
target_link_libraries(GSPCore ${OPENGL_LIBRARIES} opengl32)

