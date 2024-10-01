




if (WIN32)

    message(STATUS "Configuring for Windows")

    set(GSPCORE_PLATFORM_SOURCE 
        lib/GWindow/Win32/Win32Window.cpp
        lib/GViewCompositor/Direct3D/Direct3DViewCompositor.cpp
    )

    set(GSPCORE_PLATFORM_LIBRARIES
        d3d11
        D3DCompiler
    )

elseif (APPLE)
    message(STATUS "Configuring for macOS")

elseif(EMSCRIPTEN)
    message(STATUS "Configuring for WASM")

    set(GSPCORE_PLATFORM_SOURCE 
        lib/GWindow/Wasm/WasmWindow.cpp
        lib/GViewCompositor/WebGL/WebGLViewCompositor.cpp
    )
elseif (UNIX)
    message(STATUS "Configuring for Linux")


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
