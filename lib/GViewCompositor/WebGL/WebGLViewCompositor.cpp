#include <GViewCompositor/WebGL/WebGLViewCompositor.hpp>

#include <GLog/GLog.hpp>

WebGLViewCompositor* WebGLViewCompositor::Create()
{
    WebGLViewCompositor* compositor = new WebGLViewCompositor();

    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
    attr.alpha = EM_TRUE;
    attr.depth = EM_TRUE;
    attr.stencil = EM_FALSE;
    attr.antialias = EM_TRUE;
    attr.majorVersion = 2;
    attr.minorVersion = 0;

    compositor->context = emscripten_webgl_create_context("#canvas", &attr);
    if (compositor->context <= 0) {
        delete compositor;
        GLog::Error("Failed to create WebGL context!");
        return NULL;
    }

    emscripten_webgl_make_context_current(compositor->context);

    GLog::Info("WebGL context created!");

    return compositor;
}

void WebGLViewCompositor::Destroy(WebGLViewCompositor* compositor)
{

}

void WebGLViewCompositor::Resize(GSize size)
{
    glViewport(0, 0, size.Width, size.Height);
}

void WebGLViewCompositor::Render(GView* view)
{
    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    printf("render\n");
    emscripten_webgl_commit_frame();
}