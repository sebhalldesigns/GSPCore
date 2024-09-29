#ifndef GVIEW_HPP
#define GVIEW_HPP

#include <GSize.hpp>
#include <GColor.hpp>

class GView
{

    private:
        GColor Color;
        GSize Size;
        
    public:
        GView() = default;
        
    friend class GViewCompositorBase;
};

#endif // GVIEW_HPP