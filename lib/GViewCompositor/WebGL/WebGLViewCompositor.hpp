#ifndef WEBGLVIEWCOMPOSITOR_HPP
#define WEBGLVIEWCOMPOSITOR_HPP


#include <GViewCompositor/GViewCompositorBase.hpp>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

#include <GSize.hpp>


class WebGLViewCompositor: public GViewCompositorBase
{
    private:

        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = NULL;
        GLuint shaderProgram;
        GLuint vao, vbo;

        

        void Resize(GSize size) final;
        void Render(GView* view) final;

        static WebGLViewCompositor* Create();
        static void Destroy(WebGLViewCompositor* compositor);

        GLuint CompileShader(GLenum type, const std::string& source);
        GLuint CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);

    friend class WasmWindow;
    
};

#endif // WEBGLVIEWCOMPOSITOR_HPP