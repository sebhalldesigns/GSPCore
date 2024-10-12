#include <GWindow/GWindowBase.hpp>
#include <GWindow/X11/X11Window.hpp>

#include <map>

#include <GLog/GLog.hpp>


static Display* display = nullptr;

static std::map<Window, X11Window*> window_map;


X11Window* X11Window::Create(std::string title, GSize size)
{
    X11Window* window = new X11Window();

    display = XOpenDisplay(NULL);

    if (display == NULL)
    {
        GLog::Error("Failed to open X11 display");
        exit(EXIT_FAILURE);
    }

    int screen = DefaultScreen(display);

    window->window = XCreateSimpleWindow(display, RootWindow(display, screen), 10, 10, size.Width, size.Height, 1, BlackPixel(display, screen), WhitePixel(display, screen));

    XStoreName(display, window->window, title.c_str());


    XSelectInput(display, window->window, ExposureMask | KeyPressMask);

    XMapWindow(display, window->window);

    window_map[window->window] = window;

    GLog::Info("Created X11 Window");  

    XEvent event;
    while (true)
    {
        XNextEvent(display, &event);
        if (event.type == Expose)
        {
            GLog::Info("Window exposed");
            // Redraw window content here
        }
        else if (event.type == ConfigureNotify)
        {
            GLog::Info("Window resized");
            // Handle window resize here
        }
    }

    return window;
}

void X11Window::Destroy(X11Window* window)
{
    XDestroyWindow(display, window->window);
    window_map.erase(window->window);
    delete window;
}

void X11Window::SetDelegate(GWindowDelegate* delegate) 
{
    this->Delegate = delegate;
}

void X11Window::SetRootView(GView* view) 
{
    this->RootView = view;
    view->WindowFrame = GRect(0, 0, Size.Width, Size.Height);
    view->LayoutSubviews();
    //compositor->Render(view);
    //SwapBuffers(GetDC(hwnd));
}

void X11Window::SetTitle(std::string title) 
{
    this->Title = title;
    XStoreName(display, this->window, title.c_str());
}

void X11Window::SetSize(GSize size) 
{
    this->Size = size;
    XResizeWindow(display, this->window, size.Width, size.Height);
}

void X11Window::SetBackgroundColor(GColor color) 
{
    this->BackgroundColor = color;
}

void X11Window::SetVisibility(GWindowVisibility visibility) 
{
    switch (visibility)
    {
        case GWindowVisibility::Hidden:
            XUnmapWindow(display, this->window);
            break;
        case GWindowVisibility::Visible:
            XMapWindow(display, this->window);
            break;
        case GWindowVisibility::Maximized:
            //XMapWindow(display, this->window);
            break;
    }
}

void X11Window::SetMaximumSize(GSize size) 
{
    this->MaximumSize = size;
}

void X11Window::SetMinimumSize(GSize size) 
{
    this->MinimumSize = size;
}   

GWindowDelegate* X11Window::GetDelegate() 
{
    return this->Delegate;
}

GView* X11Window::GetRootView() 
{
    return this->RootView;
}

std::string X11Window::GetTitle() 
{
    return this->Title;
}

GSize X11Window::GetSize() 
{
    return this->Size;
}

GColor X11Window::GetBackgroundColor() 
{
    return this->BackgroundColor;
}

GWindowVisibility X11Window::GetVisibility() 
{
    return this->Visibility;
}

GSize X11Window::GetMaximumSize() 
{
    return this->MaximumSize;
}

GSize X11Window::GetMinimumSize() 
{
    return this->MinimumSize;
}

int X11Window::RunXLoop()
{
    XEvent event;
    while (true)
    {
        XNextEvent(display, &event);
        if (event.type == Expose)
        {
            GLog::Info("Window exposed");
            // Redraw window content here
        }
        else if (event.type == ConfigureNotify)
        {
            GLog::Info("Window resized");
            // Handle window resize here
        }
    }

    return EXIT_SUCCESS;
}