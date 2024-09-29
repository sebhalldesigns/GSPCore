#include <GView/GView.hpp>


GView* GView::Create()
{
    return new GView();
}


void GView::Destroy(GView* view) {
    
    if (view->Parent != nullptr) {
        view->Parent->RemoveSubview(view);
        view->Parent = nullptr;
    }

    delete view;
}

void GView::LayoutSubviews()
{
    for (GView* view : Subviews)
    {
        view->LayoutSubviews();
    }

    switch (ContentLayout)
    {
        case GViewContentLayout::Grid:
            break;
        case GViewContentLayout::Dock:
            break;
        case GViewContentLayout::VStack:
            break;
        case GViewContentLayout::HStack:
            break;
    }
}

void GView::SetBackgroundColor(GColor color)
{
    BackgroundColor = color;
}

void GView::SetSizeRequest(GSize size)
{
    SizeRequest = size;
}

void GView::SetContentLayout(GViewContentLayout layout)
{
    ContentLayout = layout;
}

void GView::SetHorizontalAlignment(GViewHorizontalAlignment alignment)
{
    HorizontalAlignment = alignment;
}

void GView::SetVerticalAlignment(GViewVerticalAlignment alignment)
{
    VerticalAlignment = alignment;
}

GRect GView::GetWindowFrame()
{
    return WindowFrame;
}

GColor GView::GetBackgroundColor()
{
    return BackgroundColor;
}

GSize GView::GetSizeRequest()
{
    return SizeRequest;
}

GViewContentLayout GView::GetContentLayout()
{
    return ContentLayout;
}

GViewHorizontalAlignment GView::GetHorizontalAlignment()
{
    return HorizontalAlignment;
}

GViewVerticalAlignment GView::GetVerticalAlignment()
{
    return VerticalAlignment;
}

void GView::AddSubview(GView* view)
{
    Subviews.push_back(view);
}

void GView::RemoveSubview(GView* view)
{
    Subviews.erase(std::remove(Subviews.begin(), Subviews.end(), view), Subviews.end());
}