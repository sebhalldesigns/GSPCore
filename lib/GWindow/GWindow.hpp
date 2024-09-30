#ifndef GWINDOW_HPP
#define GWINDOW_HPP

#ifdef _WIN32
    #include <GWindow/Win32/Win32Window.hpp>
    #define GWindow Win32Window
#elif EMSCRIPTEN
    #include <GWindow/Wasm/WasmWindow.hpp>
    #define GWindow WasmWindow
#endif


#endif // GWINDOW_HPP