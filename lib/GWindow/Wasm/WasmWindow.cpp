#include <GWindow/Wasm/WasmWindow.hpp>

#include <GLog/GLog.hpp>

static WasmWindow* GLOBAL_WINDOW = nullptr;

WasmWindow* WasmWindow::Create(std::string title, GSize size)
{

    if (GLOBAL_WINDOW != nullptr)
    {
        GLog::Error("Window already created");
        exit(EXIT_FAILURE);
    }

    WasmWindow* window = new WasmWindow();

    window->compositor = WebGLViewCompositor::Create();

    if (!window->compositor)
    {
        delete window;
        GLog::Error("Failed to start compositor");
        exit(EXIT_FAILURE);
    }

    double width, height;
    emscripten_get_element_css_size("#canvas", &width, &height);

    window->Size = GSize((float)width, (float)height);

    window->compositor->Resize(GSize((float)width, (float)height));
    window->compositor->Render(nullptr);

    GLOBAL_WINDOW = window;

    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, nullptr, EM_TRUE, EmscriptenResizeCallback);


    return window;
}

void WasmWindow::Destroy(WasmWindow* window)
{

}


EM_BOOL WasmWindow::EmscriptenResizeCallback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData)
{

    if (GLOBAL_WINDOW == nullptr)
    {
        return EM_FALSE;
    }

    GSize newSize = GSize(uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);

    GLOBAL_WINDOW->Size = newSize;

    if (GLOBAL_WINDOW->Delegate != nullptr)
    {
        GLOBAL_WINDOW->Delegate->WindowDidResize(newSize);
    }

    GLOBAL_WINDOW->compositor->Resize(newSize);

    if (GLOBAL_WINDOW->RootView != nullptr)
    {
        GLOBAL_WINDOW->RootView->WindowFrame = GRect(0, 0, uiEvent->windowInnerWidth, uiEvent->windowInnerHeight);
        GLOBAL_WINDOW->RootView->LayoutSubviews();
        GLOBAL_WINDOW->compositor->Render(GLOBAL_WINDOW->RootView);
    }

    return EM_TRUE;
}

void WasmWindow::SetDelegate(GWindowDelegate* delegate)
{
    this->Delegate = delegate;
}

void WasmWindow::SetRootView(GView* view)
{
    this->RootView = view;
    view->WindowFrame = GRect(0, 0, Size.Width, Size.Height);
    view->LayoutSubviews();
    this->compositor->Render(view);
}

void WasmWindow::SetTitle(std::string title)
{
    this->Title = title;
}

void WasmWindow::SetSize(GSize size)
{
    this->Size = size;
}

void WasmWindow::SetBackgroundColor(GColor color)
{
    this->BackgroundColor = color;
}

void WasmWindow::SetVisibility(GWindowVisibility visibility)
{
    this->Visibility = visibility;
}

void WasmWindow::SetMaximumSize(GSize size)
{
    this->MaximumSize = size;
}

void WasmWindow::SetMinimumSize(GSize size)
{
    this->MinimumSize = size;
}

GWindowDelegate* WasmWindow::GetDelegate()
{
    return this->Delegate;
}

GView* WasmWindow::GetRootView()
{
    return this->RootView;
}

std::string WasmWindow::GetTitle()
{
    return this->Title;
}

GSize WasmWindow::GetSize()
{
    return this->Size;
}

GColor WasmWindow::GetBackgroundColor()
{
    return this->BackgroundColor;
}

GWindowVisibility WasmWindow::GetVisibility()
{
    return this->Visibility;
}

GSize WasmWindow::GetMaximumSize()
{
    return this->MaximumSize;
}

GSize WasmWindow::GetMinimumSize()
{
    return this->MinimumSize;
}
