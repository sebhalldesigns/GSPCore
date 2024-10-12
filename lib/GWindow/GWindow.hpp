#ifndef GWINDOW_HPP
#define GWINDOW_HPP

#ifdef _WIN32
    #include <GWindow/Win32/Win32Window.hpp>
    #define GWindow Win32Window
#elif __linux__
    #include <GWindow/X11/X11Window.hpp>
    #define GWindow X11Window
#elif EMSCRIPTEN
    #include <GWindow/Wasm/WasmWindow.hpp>
    #define GWindow WasmWindow
#endif


#endif // GWINDOW_HPP