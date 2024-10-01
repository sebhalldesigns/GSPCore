#include <GViewCompositor/WebGL/WebGLViewCompositor.hpp>

#include <GLog/GLog.hpp>

#include <vector>
#include <string>
#include <cstdint>

struct Vertex {
    float position[2];
    float texCoord[2];
    float color[4];
    int32_t textureIndex;
};

struct ViewData {
    Vertex vertices[4];
};


const std::string vertexShaderSource = 
R"(#version 300 es

    layout(location = 0) in vec2 aPosition;
    layout(location = 1) in vec2 aTexCoord;
    layout(location = 2) in vec4 aColor;
    layout(location = 2) in int aTextureId;

    uniform vec2 viewportSize;

    out vec2 vTexCoord;
    out vec4 vColor;
    flat out int vTextureId;

    void main() {
        vec2 unit_position = aPosition/viewportSize;
        vec2 normalized_position = (unit_position*2.0)-1.0;
        normalized_position.y *= -1.0;
        gl_Position = vec4(normalized_position, 0.0, 1.0);

        vTexCoord = aTexCoord;
        vColor = aColor;
        vTextureId = aTextureId;
    }
)";

const std::string fragmentShaderSource = 
R"(#version 300 es

    precision mediump float;

    in vec2 vTexCoord;
    in vec4 vColor;
    flat in int vTextureId;

    out vec4 FragColor;

    uniform int uCurrentBatchSize;
    uniform sampler2D uTextures[8];

    void main() {

        if (vTextureId < uCurrentBatchSize) {
            switch (vTextureId) {
                case 0:
                    FragColor = texture(uTextures[0], vTexCoord);
                    break;
                case 1:
                    FragColor = texture(uTextures[1], vTexCoord);
                    break;
                case 2:
                    FragColor = texture(uTextures[2], vTexCoord);
                    break;
                case 3:
                    FragColor = texture(uTextures[3], vTexCoord);
                    break;
                case 4:
                    FragColor = texture(uTextures[4], vTexCoord);
                    break;
                case 5:
                    FragColor = texture(uTextures[5], vTexCoord);
                    break;
                case 6:
                    FragColor = texture(uTextures[6], vTexCoord);
                    break;
                case 7:
                    FragColor = texture(uTextures[7], vTexCoord);
                    break;
            }
        } else {
            FragColor = vColor;  // Use background color if no texture is active
        }
    }
)";

/*
void RenderBatch(const std::vector<GView*>& views) {
    std::vector<ViewData> viewData;
    for (GView* view : views) {
        ViewData data;
        data.texture = view->GetTexture();
        view->GetBackgroundColor(data.backgroundColor);
        view->GetVertices(data.vertices);
        viewData.push_back(data);
    }

    glBindVertexArray(vao);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, viewData.size() * sizeof(ViewData), viewData.data(), GL_DYNAMIC_DRAW);

    glUseProgram(shaderProgram);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, position));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, texCoord));

    for (size_t i = 0; i < viewData.size(); ++i) {
        glUniform4fv(glGetUniformLocation(shaderProgram, "uBackgroundColor"), 1, viewData[i].backgroundColor);
        glBindTexture(GL_TEXTURE_2D, viewData[i].texture);
        glDrawArrays(GL_TRIANGLE_FAN, i * 4, 4);
    }

    glBindVertexArray(0);
}*/

WebGLViewCompositor* WebGLViewCompositor::Create()
{
    WebGLViewCompositor* compositor = new WebGLViewCompositor();

    EmscriptenWebGLContextAttributes attr;
    emscripten_webgl_init_context_attributes(&attr);
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

    compositor->shaderProgram = compositor->CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    glGenVertexArrays(1, &compositor->vao);
    glGenBuffers(1, &compositor->vbo);

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


GLuint WebGLViewCompositor::CompileShader(GLenum type, const std::string& source) {
    GLuint shader = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(shader, 1, &src, nullptr);
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

GLuint WebGLViewCompositor::CreateShaderProgram(const std::string& vertexSource, const std::string& fragmentSource) {
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