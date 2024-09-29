#ifndef GVIEWCOMPOSITORBASE_HPP
#define GVIEWCOMPOSITORBASE_HPP

#include <GSize.hpp>
#include <GView/GView.hpp>

class GViewCompositorBase
{
    private:

        virtual void Resize(GSize size) = 0;
        virtual void Render(GView* view) = 0;

    friend class Direct3DViewCompositor;
};

#endif //GVIEWCOMPOSITORBASE_HPP