#include <GViewCompositor/WebGL/WebGLViewCompositor.hpp>

#include <GLog/GLog.hpp>

#include <vector>
#include <string>
#include <cstdint>

const uint32_t WebGLViewCompositor::QUAD_INDICES[96] = {
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

const std::string WebGLViewCompositor::vertexShaderSource = 
R"(#version 300 es

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



const std::string WebGLViewCompositor::fragmentShaderSource = 
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

        FragColor = vColor;  // Use background color if no texture is active
    }
)";


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

void WebGLViewCompositor::Destroy(WebGLViewCompositor* compositor)
{

}

void WebGLViewCompositor::Resize(GSize size)
{   
    emscripten_set_canvas_element_size("#canvas", static_cast<int>(size.Width), static_cast<int>(size.Height));
    glViewport(0, 0, static_cast<int>(size.Width), static_cast<int>(size.Height));
    glUseProgram(shaderProgram);
    glUniform2f(uViewportSize, size.Width, size.Height);
}

void WebGLViewCompositor::Render(GView* view)
{   
    
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

    emscripten_webgl_commit_frame();

}


GLuint WebGLViewCompositor::CompileShader(GLenum type, const std::string& source) {

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