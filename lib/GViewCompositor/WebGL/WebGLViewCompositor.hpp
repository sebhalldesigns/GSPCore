#ifndef WEBGLVIEWCOMPOSITOR_HPP
#define WEBGLVIEWCOMPOSITOR_HPP


#include <GViewCompositor/GViewCompositorBase.hpp>
#include <emscripten.h>
#include <emscripten/html5.h>
#include <GLES3/gl3.h>

#include <GSize.hpp>

const size_t VIEW_BATCH_SIZE = 8;
const size_t VIEW_BATCH_VERTEX_COUNT = VIEW_BATCH_SIZE * 4;

struct Vertex {
    float position[2];
    float texCoord[2];
    float color[4];
    int32_t textureIndex;
};


class WebGLViewCompositor: public GViewCompositorBase
{
    private:

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