#ifndef GVIEW_HPP
#define GVIEW_HPP

#include <GSize.hpp>
#include <GColor.hpp>
#include <GRect.hpp>

#include <vector>

enum class GViewContentLayout
{
    Grid,
    Dock,
    VStack,
    HStack
};

enum class GViewHorizontalAlignment
{
    Fill,
    Left,
    Center,
    Right
};

enum class GViewVerticalAlignment
{
    Fill,
    Top,
    Center,
    Bottom
};

enum class GDockPosition
{
    Top,
    Bottom,
    Left,
    Right
};

class GView
{

    private:

        GRect WindowFrame;

        GColor BackgroundColor;
        GSize SizeRequest;

        GViewContentLayout ContentLayout;

        GViewHorizontalAlignment HorizontalAlignment;
        GViewVerticalAlignment VerticalAlignment;

        GDockPosition DockPosition;

        std::vector<GView*> Subviews;

        uintptr_t Texture = 0;
        
        GView* Parent = nullptr;

        void LayoutSubviews();

        GView() = default;
        ~GView() = default;

    public:
        
        static GView* Create();
        static void Destroy(GView* view);

        void SetBackgroundColor(GColor color);
        void SetSizeRequest(GSize size);
        void SetContentLayout(GViewContentLayout layout);
        void SetHorizontalAlignment(GViewHorizontalAlignment alignment);
        void SetVerticalAlignment(GViewVerticalAlignment alignment);
        void SetDockPosition(GDockPosition position);

        GRect GetWindowFrame();

        GColor GetBackgroundColor();
        GSize GetSizeRequest();
        GViewContentLayout GetContentLayout();
        GViewHorizontalAlignment GetHorizontalAlignment();
        GViewVerticalAlignment GetVerticalAlignment();
        GDockPosition GetDockPosition();

        void AddSubview(GView* view);
        void RemoveSubview(GView* view);
        
    friend class Direct3DViewCompositor;
    friend class WebGLViewCompositor;
    friend class OpenGLViewCompositor;
    friend class Win32Window;
    friend class WasmWindow;
};

#endif // GVIEW_HPP