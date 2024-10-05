#ifndef GVIEWCOMPOSITORBASE_HPP
#define GVIEWCOMPOSITORBASE_HPP

#include <GSize.hpp>
#include <GView/GView.hpp>

struct Vertex {
    float position[2];
    float texCoord[2];
    float color[4];
    int32_t textureIndex;
};


class GViewCompositorBase
{
    private:

        virtual void Resize(GSize size) = 0;
        virtual void Render(GView* view) = 0;

    friend class Direct3DViewCompositor;
    friend class WebGLViewCompositor;
};

#endif //GVIEWCOMPOSITORBASE_HPP