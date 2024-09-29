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

class GView
{

    private:

        GRect WindowFrame;

        GColor BackgroundColor;
        GSize SizeRequest;

        GViewContentLayout ContentLayout;

        GViewHorizontalAlignment HorizontalAlignment;
        GViewVerticalAlignment VerticalAlignment;

        std::vector<GView*> Subviews;
        
        GView* Parent = nullptr;

        void LayoutSubviews();

        GView() = default;
        ~GView() = default;

    public:
        
        GView* Create();
        void Destroy(GView* view);

        void SetBackgroundColor(GColor color);
        void SetSizeRequest(GSize size);
        void SetContentLayout(GViewContentLayout layout);
        void SetHorizontalAlignment(GViewHorizontalAlignment alignment);
        void SetVerticalAlignment(GViewVerticalAlignment alignment);

        GRect GetWindowFrame();

        GColor GetBackgroundColor();
        GSize GetSizeRequest();
        GViewContentLayout GetContentLayout();
        GViewHorizontalAlignment GetHorizontalAlignment();
        GViewVerticalAlignment GetVerticalAlignment();

        void AddSubview(GView* view);
        void RemoveSubview(GView* view);
        
    friend class Direct3DViewCompositor;
    friend class Win32Window;
};

#endif // GVIEW_HPP