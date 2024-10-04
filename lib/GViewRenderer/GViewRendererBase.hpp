#ifndef GVIEWRENDERERBASE_HPP
#define GVIEWRENDERERBASE_HPP

#include <GSize.hpp>
#include <GView/GView.hpp>

class GViewRendererBase
{
    private:

        virtual void Resize(GSize size) = 0;
        virtual void Render(GView* view) = 0;

    friend class Direct3DViewCompositor;
    friend class WebGLViewCompositor;
};

#endif //GVIEWRENDERERBASE_HPP