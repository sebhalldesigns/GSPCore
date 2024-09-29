#ifndef GWINDOWDELEGATE_HPP
#define GWINDOWDELEGATE_HPP

#include <GSize.hpp>

class GWindowDelegate
{
    public:
        GWindowDelegate() = default;
        virtual ~GWindowDelegate() = default;

        virtual void WindowDidResize(GSize size) = 0;
        virtual void WindowDidClose() = 0;
};

#endif // GWINDOWDELEGATE_HPP