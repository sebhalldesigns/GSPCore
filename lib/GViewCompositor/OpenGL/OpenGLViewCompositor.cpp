#include <GViewCompositor/OpenGL/OpenGLViewCompositor.hpp>

#include <GLog/GLog.hpp>

bool OpenGLViewCompositor::hasInitialized = false;
uintptr_t OpenGLViewCompositor::currentContext = NULL;

const uint32_t OpenGLViewCompositor::QUAD_INDICES[96] = {
     0,  1,  2,
     0,  2,  3,
     4,  5,  6,
     4,  6,  7,
     8,  9, 10,
     8, 10, 11,
    12, 13, 14,
    12, 14, 15,
    16, 17, 18,
    16, 18, 19,
    20, 21, 22,
    20, 22, 23,
    24, 25, 26,
    24, 26, 27,
    28, 29, 30,
    28, 30, 31,
    32, 33, 34,
    32, 34, 35,
    36, 37, 38,
    36, 38, 39,
    40, 41, 42,
    40, 42, 43,
    44, 45, 46,
    44, 46, 47,
    48, 49, 50,
    48, 50, 51,
    52, 53, 54,
    52, 54, 55,
    56, 57, 58,
    56, 58, 59,
    60, 61, 62,
    60, 62, 63
};

const std::string OpenGLViewCompositor::vertexShaderSource = 
R"(#version 330 core

    layout(location = 0) in vec2 aPosition;
    layout(location = 1) in vec2 aTexCoord;
    layout(location = 2) in vec4 aColor;
    layout(location = 3) in int aTextureId;

    uniform vec2 uViewportSize;

    out vec2 vTexCoord;
    out vec4 vColor;
    flat out int vTextureId;

    void main() {
        vec2 unit_position = aPosition/uViewportSize;
        vec2 normalized_position = (unit_position*2.0)-1.0;
        normalized_position.y *= -1.0;
        gl_Position = vec4(normalized_position, 0.0, 1.0);

        vTexCoord = aTexCoord;
        vColor = aColor;
        vTextureId = aTextureId;
    }
)";



const std::string OpenGLViewCompositor::fragmentShaderSource = 
R"(#version 330 core

    in vec2 vTexCoord;
    in vec4 vColor;
    flat in int vTextureId;

    out vec4 FragColor;

    uniform int uCurrentBatchSize;
    uniform sampler2D uTextures[16];

    void main() {

        if (vTextureId < uCurrentBatchSize) {
            FragColor = texture(uTextures[vTextureId], vTexCoord);
        } else {
            FragColor = vColor;
        }

        FragColor = vColor; 
    }
)";



void OpenGLViewCompositor::InitializeOpenGL()
{

    WNDCLASSA window_class = { };
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = DefWindowProcA;
    window_class.hInstance = GetModuleHandle(0);
    window_class.lpszClassName = "Dummy_WGL_djuasiodwa";


    if (!RegisterClassA(&window_class)) {
        GLog::Error("Failed to register dummy OpenGL window.");
        return;
    }

    HWND dummy_window = CreateWindowExA(
        0,
        window_class.lpszClassName,
        "Dummy OpenGL Window",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        window_class.hInstance,
        0);

    if (!dummy_window) {
        GLog::Error("Failed to create dummy OpenGL window.");
        return;
    }

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd = {};
        pfd.nSize = sizeof(pfd);
        pfd.nVersion = 1;
        pfd.iPixelType = PFD_TYPE_RGBA;
        pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
        pfd.cColorBits = 32;
        pfd.cAlphaBits = 8;
        pfd.iLayerType = PFD_MAIN_PLANE;
        pfd.cDepthBits = 24;
        pfd.cStencilBits = 8;

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    if (!pixel_format) {
        GLog::Error("Failed to find a suitable pixel format.");
        return;
    }

    if (!SetPixelFormat(dummy_dc, pixel_format, &pfd)) {
        GLog::Error("Failed to set the pixel format.");
        return;
    }

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    if (!dummy_context) {
        GLog::Error("Failed to create a dummy OpenGL rendering context.");
        return;
    }

    if (!wglMakeCurrent(dummy_dc, dummy_context)) {
        GLog::Error("Failed to activate dummy OpenGL rendering context.");
        return;
    }

    if (!gladLoadGL()) {
        GLog::Error("Failed to initialize OpenGL 3.3 extensions.");
        return;
    }

    if (!gladLoadWGL(dummy_dc)) {
        GLog::Error("Failed to initialize WGL extensions.");
        return;
    }

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);

    hasInitialized = true;
}

void OpenGLViewCompositor::Resize(GSize size)
{
    glViewport(0, 0, size.Width, size.Height);
    glUseProgram(shaderProgram);
    glUniform2f(uViewportSize, size.Width, size.Height);
}

void OpenGLViewCompositor::Render(GView* view)
{
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glClearColor(0.0f, 0.2f, 0.4f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    
    std::vector<GView*> viewStack;
    std::vector<GView*> stack;
    stack.push_back(view);

    while (!stack.empty()) {
        GView* currentView = stack.back();
        stack.pop_back();
        viewStack.push_back(currentView);

        const std::vector<GView*>& subviews = currentView->Subviews;
        for (auto it = subviews.rbegin(); it != subviews.rend(); ++it) {
            stack.push_back(*it);
        }
    }
    size_t packedViewIndex = 0;

    for (size_t viewIndex = 0; viewIndex < viewStack.size(); ++viewIndex) {
        GView* currentView = viewStack[viewIndex];
        int32_t textureIndex = VIEW_BATCH_SIZE;

        GLuint texture = currentView->Texture;

        if (texture != 0) {
            glActiveTexture(GL_TEXTURE0 + packedViewIndex);
            glBindTexture(GL_TEXTURE_2D, texture);
            textureIndex = packedViewIndex;
        }

        GRect rect = viewStack[viewIndex]->WindowFrame;
        GColor color = viewStack[viewIndex]->BackgroundColor;

        vertices[packedViewIndex * 4] = { {rect.X, rect.Y}, {0.0f, 0.0f}, {color.Red, color.Green, color.Blue, color.Alpha}, textureIndex };
        vertices[(packedViewIndex * 4) + 1] = { {rect.X + rect.Width, rect.Y}, {1.0f, 0.0f}, {color.Red, color.Green, color.Blue, color.Alpha}, textureIndex };
        vertices[(packedViewIndex * 4) + 2] = { {rect.X + rect.Width, rect.Y + rect.Height}, {1.0f, 1.0f}, {color.Red, color.Green, color.Blue, color.Alpha}, textureIndex };
        vertices[(packedViewIndex * 4) + 3] = { {rect.X, rect.Y + rect.Height}, {0.0f, 1.0f}, {color.Red, color.Green, color.Blue, color.Alpha}, textureIndex };

        if (packedViewIndex >= (VIEW_BATCH_SIZE - 1) || (viewIndex == (viewStack.size() - 1))) {

            glUseProgram(shaderProgram);
            glBindVertexArray(vao);
            glBindBuffer(GL_ARRAY_BUFFER, vbo);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex) * VIEW_BATCH_VERTEX_COUNT, vertices);

            glUniform1i(uCurrentBatchSize, packedViewIndex + 1);
            glDrawElements(GL_TRIANGLES, (packedViewIndex + 1) * 6, GL_UNSIGNED_INT, nullptr);

            glBindVertexArray(0);

            
            //printf("rendered %zu views\n", packedViewIndex + 1);
            
            packedViewIndex = 0;

        } else {
            packedViewIndex++;
        }
    }

}

OpenGLViewCompositor* OpenGLViewCompositor::Create(uintptr_t windowHandle)
{
    if (!hasInitialized)
    {
        InitializeOpenGL();
    }

    if (!hasInitialized)
    {
        GLog::Error("Failed to start OpenGL!");
        return nullptr;
    }

    OpenGLViewCompositor* compositor = new OpenGLViewCompositor();

    compositor->windowHandle = windowHandle;

    HDC real_dc = GetDC((HWND)windowHandle);

     // Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(real_dc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!num_formats) {
        GLog::Error("Failed to set the OpenGL 3.3 pixel format.");
        delete compositor;
        return nullptr;
    }

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(real_dc, pixel_format, sizeof(pfd), &pfd);
    if (!SetPixelFormat(real_dc, pixel_format, &pfd)) {
        GLog::Error("Failed to set the OpenGL 3.3 pixel format.");
        delete compositor;
        return nullptr;
    }

    // Specify that we want to create an OpenGL 3.3 core profile context
    int gl33_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    compositor->context = (uintptr_t)wglCreateContextAttribsARB(real_dc, 0, gl33_attribs);
    if (!compositor->context) {
        GLog::Error("Failed to create OpenGL 3.3 context.");
        delete compositor;
        return nullptr;
    }

    if (!wglMakeCurrent(real_dc,  (HGLRC)compositor->context)) {
        GLog::Error("Failed to activate OpenGL 3.3 rendering context.");
        delete compositor;
        return nullptr;
    }

    GLog::Info("OpenGL context created!");

    compositor->shaderProgram = compositor->CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    glUseProgram(compositor->shaderProgram);

    compositor->uViewportSize = glGetUniformLocation(compositor->shaderProgram, "uViewportSize");
    compositor->uCurrentBatchSize = glGetUniformLocation(compositor->shaderProgram, "uCurrentBatchSize");

    glGenVertexArrays(1, &compositor->vao);
    glGenBuffers(1, &compositor->vbo);
    glGenBuffers(1, &compositor->ebo);

    glBindVertexArray(compositor->vao);

    glBindBuffer(GL_ARRAY_BUFFER, compositor->vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * VIEW_BATCH_VERTEX_COUNT, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, compositor->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(QUAD_INDICES), QUAD_INDICES, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(2*sizeof(float)));
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(4*sizeof(float)));
    glEnableVertexAttribArray(2);

    glVertexAttribIPointer(3, 1, GL_INT, sizeof(Vertex), (void*)(8*sizeof(float)));
    glEnableVertexAttribArray(3);

    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);



    return compositor;
}

void OpenGLViewCompositor::Destroy(OpenGLViewCompositor* compositor)
{
    delete compositor;
}

GLuint OpenGLViewCompositor::CompileShader(GLenum type, const std::string& source)
{
    
    GLuint shader = glCreateShader(type);
    const char* shaderSource = source.c_str();
    glShaderSource(shader, 1, &shaderSource, nullptr);
    glCompileShader(shader);

    GLint status;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

    if (status != GL_TRUE) {
        char buffer[512];
        glGetShaderInfoLog(shader, 512, nullptr, buffer);
        GLog::Error("Shader compilation failed: %s", buffer);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

GLuint OpenGLViewCompositor::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
    GLuint vertexShader = CompileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint status;
    glGetProgramiv(program, GL_LINK_STATUS, &status);
    
    if (status != GL_TRUE) {
        char buffer[512];
        glGetProgramInfoLog(program, 512, nullptr, buffer);
        GLog::Error("Program linking failed: %s", buffer);
        glDeleteProgram(program);
        return 0;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}