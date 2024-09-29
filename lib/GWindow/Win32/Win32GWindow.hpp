#ifndef WIN32GWINDOW_HPP
#define WIN32GWINDOW_HPP

#include <Windows.h>

class Win32GWindow
{
    private:
        HWND hwnd;
        HGLRC glContext;

    public:
        Win32GWindow() = default;
        Win32GWindow(const char* title, float width, float height);
        static void Poll();
};

#endif // WIN32GWINDOW_HPP