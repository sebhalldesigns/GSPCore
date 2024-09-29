#ifndef WASMGWINDOW_HPP
#define WASMGWINDOW_HPP

#include <Windows.h>

class WasmGWindow
{
    private:
        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context;
    public:
        WasmGWindow() = default;
        WasmGWindow(const char* title, float width, float height);
        static void Poll();
};

#endif // WASMGWINDOW_HPP