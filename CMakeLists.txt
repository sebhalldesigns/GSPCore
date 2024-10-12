




if (WIN32)

    message(STATUS "Configuring for Windows")

    set(GSPCORE_PLATFORM_SOURCE 
        lib/GWindow/Win32/Win32Window.cpp
        lib/GViewCompositor/OpenGL/OpenGLViewCompositor.cpp
        lib/GViewCompositor/OpenGL/glad.c
        lib/GViewCompositor/OpenGL/glad_wgl.c
    )

    set(GSPCORE_PLATFORM_LIBRARIES
        opengl32
    )

elseif (APPLE)
    message(STATUS "Configuring for macOS")

elseif(EMSCRIPTEN)
    message(STATUS "Configuring for WASM")

    set(GSPCORE_PLATFORM_SOURCE 
        lib/GWindow/Wasm/WasmWindow.cpp
        lib/GViewCompositor/WebGL/WebGLViewCompositor.cpp
    )

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS}  -s \"FETCH=1\" -s \"ALLOW_MEMORY_GROWTH=1\"-s \"USE_WEBGL2=1\" -s \"MIN_WEBGL_VERSION=2\"")
elseif (UNIX)
    message(STATUS "Configuring for Linux")

    set(GSPCORE_PLATFORM_SOURCE 
        lib/GWindow/X11/X11Window.cpp
        #lib/GViewCompositor/OpenGL/OpenGLViewCompositor.cpp
        #lib/GViewCompositor/OpenGL/glad.c
        #lib/GViewCompositor/OpenGL/glad_wgl.c
    )

    set(GSPCORE_PLATFORM_LIBRARIES
        X11
    )

else()
    message(FATAL_ERROR "Unknown or unsupported platform!")
endif()

# Add the executable
add_library(GSPCore STATIC
    lib/GApplication/GApplication.cpp
    lib/GLog/GLog.cpp
    lib/GView/GView.cpp
    ${GSPCORE_PLATFORM_SOURCE}
)

target_include_directories(GSPCore PRIVATE lib)

target_link_libraries(GSPCore ${GSPCORE_PLATFORM_LIBRARIES})


