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
            // Implement grid layout logic here
            break;
        case GViewContentLayout::Dock:
            {
                size_t children_consumed = 0;

            float consumed_left = 0.0f;
            float consumed_top = 0.0f;
            float consumed_right = 0.0f;
            float consumed_bottom = 0.0f;

            // If the view has children, add them to the stack
            for (GView* child_view : Subviews) {

                if (children_consumed == Subviews.size() - 1) {
                    // this is the last child in the dock
                    child_view->WindowFrame = GRect {
                        WindowFrame.X + consumed_left,
                        WindowFrame.Y + consumed_top,
                        WindowFrame.Width - (consumed_left + consumed_right),
                        WindowFrame.Height - (consumed_top + consumed_bottom)
                    };
                } else {
                    switch (child_view->DockPosition) {
                        case GDockPosition::Left:
                            child_view->WindowFrame = GRect {
                                WindowFrame.X + consumed_left,
                                WindowFrame.Y + consumed_top,
                                child_view->SizeRequest.Width,
                                WindowFrame.Height - (consumed_top + consumed_bottom)
                            };

                            consumed_left += child_view->SizeRequest.Width;
                            break;
                        case GDockPosition::Top:
                            child_view->WindowFrame = GRect {
                                WindowFrame.X + consumed_left,
                                WindowFrame.Y + consumed_top,
                                WindowFrame.Width - (consumed_left + consumed_right),
                                child_view->SizeRequest.Height
                            };

                            consumed_top += child_view->SizeRequest.Height;
                            break;
                        case GDockPosition::Right:
                            child_view->WindowFrame = GRect {
                                WindowFrame.X + WindowFrame.Width - (consumed_right + child_view->SizeRequest.Width),
                                WindowFrame.Y + consumed_top,
                                child_view->SizeRequest.Width,
                                WindowFrame.Height - (consumed_top + consumed_bottom)
                            };

                            consumed_right += child_view->SizeRequest.Width;
                            break;
                        case GDockPosition::Bottom:
                            child_view->WindowFrame = GRect {
                                WindowFrame.X + consumed_left,
                                WindowFrame.Y + WindowFrame.Height - (consumed_bottom + child_view->SizeRequest.Height),
                                WindowFrame.Width - (consumed_left + consumed_right),
                                child_view->SizeRequest.Height
                            };

                            consumed_bottom += child_view->SizeRequest.Height;
                            break;
                    }
                }

                // Assuming you have a stack to push the child views onto
                // stack.push(child_view);

                children_consumed += 1;
            }
            }
            
            
            break;
        case GViewContentLayout::VStack:
            // Implement vertical stack layout logic here
            break;
        case GViewContentLayout::HStack:
            // Implement horizontal stack layout logic here
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

void GView::SetDockPosition(GDockPosition position)
{
    DockPosition = position;
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

GDockPosition GView::GetDockPosition()
{
    return DockPosition;
}

void GView::AddSubview(GView* view)
{
    Subviews.push_back(view);
}

void GView::RemoveSubview(GView* view)
{
    Subviews.erase(std::remove(Subviews.begin(), Subviews.end(), view), Subviews.end());
}