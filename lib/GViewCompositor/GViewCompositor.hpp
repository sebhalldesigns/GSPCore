#ifndef GVIEWCOMPOSITOR_HPP
#define GVIEWCOMPOSITOR_HPP

#ifdef _WIN32
    #include <GViewCompositor/Direct3D/Direct3DViewCompositor.hpp>
    #define GViewCompositor Direct3DViewCompositor
#elif EMSCRIPTEN
    #include <GViewCompositor/WebGL/WebGLViewCompositor.hpp>
    #define GViewCompositor WebGLViewCompositor
#endif


#endif //GVIEWCOMPOSITOR_HPP