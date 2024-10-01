#include <GWindow/GWindowBase.hpp>
#include <GWindow/Win32/Win32Window.hpp>


#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif 

#include <Windows.h>
#include <GL/gl.h>
#include <cstdio>
#include <string>
#include <map>

#include <GLog/GLog.hpp>


// LOCAL FUNCTIONS

// function to convert utf8 std::string to std::wstring
std::wstring utf8_to_wstring(const std::string& str);

// STATIC VARIABLES

const static LPCWSTR RUNTIME_WINDOW_CLASS = L"GSPCoreWindowClass";

static WNDCLASSW runtime_window_class = { };
static bool hasSetup = false;

static std::map<HWND, Win32Window*> window_map;


Win32Window* Win32Window::Create(std::string title, GSize size)
{   

    Win32Window* window = new Win32Window();

    if (!hasSetup) 
    {
        runtime_window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
        runtime_window_class.lpfnWndProc = Win32Window::WindowProc;
        runtime_window_class.hInstance = GetModuleHandle(0);
        runtime_window_class.lpszClassName = RUNTIME_WINDOW_CLASS;
        runtime_window_class.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);

        if (RegisterClassW(&runtime_window_class))
        {
           hasSetup = true;
        }
        else 
        {   
            GLog::Error("Failed to register window class");
            exit(EXIT_FAILURE);
        }
        
    }

    window->hwnd = CreateWindowW(
        RUNTIME_WINDOW_CLASS,
        utf8_to_wstring(title).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, (int)size.Width, (int)size.Height,
        NULL,
        NULL,
        GetModuleHandle(nullptr),
        NULL
    );

    if (window->hwnd == NULL) 
    {
        return nullptr;
    }


    

    window->compositor = Direct3DViewCompositor::Create(window->hwnd);

    if (!window->compositor)
    {
        delete window;
        GLog::Error("Failed to create compositor");
        exit(EXIT_FAILURE);
    }

    // update properties and add to map
    window->Title = title;
    window->Size = size;
    window->Visibility = GWindowVisibility::Visible;

    window_map[window->hwnd] = window;

    ShowWindow(window->hwnd, SW_SHOW);
    UpdateWindow(window->hwnd);

    return window;
}

void Win32Window::Destroy(Win32Window* window)
{
    DestroyWindow(window->hwnd);
    window_map.erase(window->hwnd); 
    delete window;
}


LRESULT CALLBACK Win32Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{

    if (window_map.find(hwnd) == window_map.end()) 
    {
        return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

    Win32Window* window = window_map[hwnd];

    switch (uMsg) 
    {
        case WM_SHOWWINDOW:
        {
            if (window->RootView != nullptr)
            {
                window->compositor->Render(window->RootView);
            }
            return 0;
        }
        case WM_SIZE: 
        {
            GSize newSize = GSize((float)LOWORD(lParam), (float)HIWORD(lParam));

            window->Size = newSize;

            if (window->Delegate != nullptr)
            {
                window->Delegate->WindowDidResize(newSize);
            }
            
            window->compositor->Resize(newSize);

            if (window->RootView != nullptr)
            {
                window->RootView->WindowFrame = GRect(0, 0, (float)LOWORD(lParam), (float)HIWORD(lParam));
                window->RootView->LayoutSubviews();
            }

            PostMessage(hwnd, WM_PAINT, 0, 0);
            return 0;
        }
            
        case WM_PAINT: 
        {
            if (window->RootView != nullptr)
            {
                window->compositor->Render(window->RootView);
            }

            ValidateRect(hwnd, NULL);
            return 0;
        }

        case WM_CLOSE: 
        {
            PostQuitMessage(0);
            return 0;    
        }
            
        default:
        {
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
        }
    }
}

void Win32Window::SetDelegate(GWindowDelegate* delegate) {
    this->Delegate = delegate;
}

void Win32Window::SetRootView(GView* view) 
{
    this->RootView = view;
}

void Win32Window::SetTitle(std::string title) 
{
    this->Title = title;
    SetWindowTextW(this->hwnd, utf8_to_wstring(title).c_str());
}

void Win32Window::SetSize(GSize size) 
{
    this->Size = size;
    SetWindowPos(this->hwnd, HWND_TOP, 0, 0, (int)size.Width, (int)size.Height, SWP_NOMOVE);
}

void Win32Window::SetBackgroundColor(GColor color) 
{
    this->BackgroundColor = color;
    //glClearColor(color.Red, color.Green, color.Blue, color.Alpha);
}

void Win32Window::SetVisibility(GWindowVisibility visibility) 
{
    this->Visibility = visibility;
    switch (visibility) 
    {
        case GWindowVisibility::Visible:
            ShowWindow(this->hwnd, SW_SHOW);
            break;
        case GWindowVisibility::Hidden:
            ShowWindow(this->hwnd, SW_HIDE);
            break;
        case GWindowVisibility::Minimized:
            ShowWindow(this->hwnd, SW_MINIMIZE);
            break;
        case GWindowVisibility::Maximized:
            ShowWindow(this->hwnd, SW_MAXIMIZE);
            break;
    }
}

void Win32Window::SetMaximumSize(GSize size) 
{
    this->MaximumSize = size;
}

void Win32Window::SetMinimumSize(GSize size)
{
    this->MinimumSize = size;
}

GWindowDelegate* Win32Window::GetDelegate() 
{
    return this->Delegate;
}

GView* Win32Window::GetRootView() 
{
    return this->RootView;
}

std::string Win32Window::GetTitle() 
{
    return this->Title;
}

GSize Win32Window::GetSize() 
{
    return this->Size;
}

GColor Win32Window::GetBackgroundColor() 
{
    return this->BackgroundColor;
}

GWindowVisibility Win32Window::GetVisibility() 
{
    return this->Visibility;
}

GSize Win32Window::GetMaximumSize() 
{
    return this->MaximumSize;
}

GSize Win32Window::GetMinimumSize()
{
    return this->MinimumSize;
}

std::wstring utf8_to_wstring(const std::string& str) 
{
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}