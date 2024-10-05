#ifndef GVIEWCOMPOSITOR_HPP
#define GVIEWCOMPOSITOR_HPP

#ifdef _WIN32
    #include <GViewCompositor/OpenGL/OpenGLViewCompositor.hpp>
    #define GViewCompositor OpenGLViewCompositor
#elif EMSCRIPTEN
    #include <GViewCompositor/WebGL/WebGLViewCompositor.hpp>
    #define GViewCompositor WebGLViewCompositor
#endif


#endif //GVIEWCOMPOSITOR_HPP