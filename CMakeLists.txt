

# Add the executable
add_library(GSPCore STATIC
    lib/GApplication/GApplication.cpp
    lib/GLog/GLog.cpp
    lib/GWindow/Win32/Win32Window.cpp
    lib/GViewCompositor/Direct3D/Direct3DViewCompositor.cpp
)

# Include directories for OpenGL and GLEW
target_include_directories(GSPCore PRIVATE ${OPENGL_INCLUDE_DIRS} lib)

# Link the executable with the necessary libraries
target_link_libraries(GSPCore d3d11 D3DCompiler)

