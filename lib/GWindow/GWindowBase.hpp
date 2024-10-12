#ifndef GWINDOWBASE_HPP
#define GWINDOWBASE_HPP

#include <GSize.hpp>
#include <GColor.hpp>
#include <GWindowVisibility.hpp>

#include <GWindowDelegate/GWindowDelegate.hpp>

#include <GView/GView.hpp>

#include <string>

class GWindowBase
{
    private:
        
        std::string Title;

        GSize Size;
        GSize MaximumSize;
        GSize MinimumSize;

        GColor BackgroundColor;
        GWindowVisibility Visibility;

        GWindowDelegate* Delegate;
        GView* RootView;

        GWindowBase() = default;
        ~GWindowBase() = default;

    public:

        virtual void SetDelegate(GWindowDelegate* delegate) = 0; 
        virtual void SetRootView(GView* view) = 0;
        virtual void SetTitle(std::string title) = 0;
        virtual void SetSize(GSize size) = 0;
        virtual void SetBackgroundColor(GColor color) = 0;
        virtual void SetVisibility(GWindowVisibility visibility) = 0;
        virtual void SetMaximumSize(GSize size) = 0;
        virtual void SetMinimumSize(GSize size) = 0;
        
        virtual GWindowDelegate* GetDelegate() = 0;
        virtual GView* GetRootView() = 0;
        virtual std::string GetTitle() = 0;
        virtual GSize GetSize() = 0;
        virtual GColor GetBackgroundColor() = 0;
        virtual GWindowVisibility GetVisibility() = 0;
        virtual GSize GetMaximumSize() = 0;
        virtual GSize GetMinimumSize() = 0;

    friend class Win32Window;
    friend class X11Window;
    friend class WasmWindow;
};


#endif // GWINDOWBASE_HPP