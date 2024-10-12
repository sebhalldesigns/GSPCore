#ifndef OPENGLVIEWCOMPOSITOR_HPP
#define OPENGLVIEWCOMPOSITOR_HPP


#include <GViewCompositor/GViewCompositorBase.hpp>
#include <string>

#ifdef _WIN32
    #include <windows.h>
    #include "glad.h"
    #include "glad_wgl.h"
#elif __linux__
    #include <GL/gl.h>
#endif


#include <GSize.hpp>

const size_t VIEW_BATCH_SIZE = 16;
const size_t VIEW_BATCH_VERTEX_COUNT = VIEW_BATCH_SIZE * 4;


class OpenGLViewCompositor: public GViewCompositorBase
{
    private:


        const static uint32_t QUAD_INDICES[96];
        const static std::string vertexShaderSource;
        const static std::string fragmentShaderSource;


        static bool hasInitialized;
        static uintptr_t currentContext;

        uintptr_t windowHandle = NULL;
        uintptr_t context = NULL;
        
        GLuint shaderProgram;
        GLuint vao, vbo, ebo;

        int32_t uViewportSize;
        int32_t uCurrentBatchSize;

        Vertex vertices[VIEW_BATCH_VERTEX_COUNT];

        static void InitializeOpenGL();

        void Resize(GSize size) final;
        void Render(GView* view) final;

        static OpenGLViewCompositor* Create(uintptr_t windowHandle);
        static void Destroy(OpenGLViewCompositor* compositor);

        GLuint CompileShader(GLenum type, const std::string& source);
        GLuint CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);

    friend class Win32Window;
    friend class X11Window;
    friend class WaylandWindow;
    
};

#endif // OPENGLVIEWCOMPOSITOR_HPP