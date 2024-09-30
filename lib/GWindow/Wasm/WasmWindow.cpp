#include <GWindow/Wasm/WasmWindow.hpp>

#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

void render() {
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    printf("render\n");
    emscripten_webgl_commit_frame();
}

WasmWindow* WasmWindow::Create(std::string title, GSize size)
{

    WasmWindow* window = new WasmWindow();


    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.alpha = EM_TRUE;
    attr.depth = EM_TRUE;
    attr.stencil = EM_FALSE;
    attr.antialias = EM_TRUE;
    attr.majorVersion = 2;
    attr.minorVersion = 0;

    window->context = emscripten_webgl_create_context("#canvas", &attr);
    if (window->context <= 0) {
        delete window;
        printf("Failed to create WebGL context!\n");
        return NULL;
    }

    emscripten_webgl_make_context_current(window->context);

    emscripten_set_main_loop(render, 0, 1);
     emscripten_webgl_commit_frame();

     return window;
}

void WasmWindow::Destroy(WasmWindow* window)
{

}

void WasmWindow::SetDelegate(GWindowDelegate* delegate)
{
    this->Delegate = delegate;
}

void WasmWindow::SetRootView(GView* view)
{
    this->RootView = view;
}

void WasmWindow::SetTitle(std::string title)
{
    this->Title = title;
}

void WasmWindow::SetSize(GSize size)
{
    this->Size = size;
}

void WasmWindow::SetBackgroundColor(GColor color)
{
    this->BackgroundColor = color;
}

void WasmWindow::SetVisibility(GWindowVisibility visibility)
{
    this->Visibility = visibility;
}

void WasmWindow::SetMaximumSize(GSize size)
{
    this->MaximumSize = size;
}

void WasmWindow::SetMinimumSize(GSize size)
{
    this->MinimumSize = size;
}

GWindowDelegate* WasmWindow::GetDelegate()
{
    return this->Delegate;
}

GView* WasmWindow::GetRootView()
{
    return this->RootView;
}

std::string WasmWindow::GetTitle()
{
    return this->Title;
}

GSize WasmWindow::GetSize()
{
    return this->Size;
}

GColor WasmWindow::GetBackgroundColor()
{
    return this->BackgroundColor;
}

GWindowVisibility WasmWindow::GetVisibility()
{
    return this->Visibility;
}

GSize WasmWindow::GetMaximumSize()
{
    return this->MaximumSize;
}

GSize WasmWindow::GetMinimumSize()
{
    return this->MinimumSize;
}