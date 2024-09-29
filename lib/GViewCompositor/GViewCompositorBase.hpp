#ifndef GVIEWCOMPOSITORBASE_HPP
#define GVIEWCOMPOSITORBASE_HPP

#include <GSize.hpp>

class GViewCompositorBase
{
    private:

        virtual void Resize(GSize size) = 0;
        virtual void Render() = 0;

    friend class Direct3DViewCompositor;
};

#endif //GVIEWCOMPOSITORBASE_HPP