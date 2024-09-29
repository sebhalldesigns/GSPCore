#include <GWindow/Wasm/WasmGWindow.hpp>

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

void render() {
    glClearColor(0.0f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}


WasmGWindow::WasmGWindow(const char* title, float width, float height)
{
    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.alpha = EM_TRUE;
    attr.depth = EM_TRUE;
    attr.stencil = EM_FALSE;
    attr.antialias = EM_TRUE;
    attr.majorVersion = 2;
    attr.minorVersion = 0;

    context = emscripten_webgl_create_context("#canvas", &attr);
    if (context <= 0) {
        printf("Failed to create WebGL context!\n");
        return 1;
    }

    emscripten_webgl_make_context_current(context);

    emscripten_set_main_loop(render, 0, 1);
}

void WasmGWindow::Poll() {
    emscripten_webgl_commit_frame();
}
