#ifndef WEBGLVIEWCOMPOSITOR_HPP
#define WEBGLVIEWCOMPOSITOR_HPP


#include <GViewCompositor/GViewCompositorBase.hpp>

#include <string>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

#include <GSize.hpp>

const size_t VIEW_BATCH_SIZE = 8;
const size_t VIEW_BATCH_VERTEX_COUNT = VIEW_BATCH_SIZE * 4;


class WebGLViewCompositor: public GViewCompositorBase
{
    private:

        const static uint32_t QUAD_INDICES[96];
        const static std::string vertexShaderSource;
        const static std::string fragmentShaderSource;


        EMSCRIPTEN_WEBGL_CONTEXT_HANDLE context = NULL;
        GLuint shaderProgram;
        GLuint vao, vbo, ebo;

        int32_t uViewportSize;
        int32_t uCurrentBatchSize;

        Vertex vertices[VIEW_BATCH_VERTEX_COUNT];

        void Resize(GSize size) final;
        void Render(GView* view) final;

        static WebGLViewCompositor* Create();
        static void Destroy(WebGLViewCompositor* compositor);

        GLuint CompileShader(GLenum type, const std::string& source);
        GLuint CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);

    friend class WasmWindow;
    
};

#endif // WEBGLVIEWCOMPOSITOR_HPP