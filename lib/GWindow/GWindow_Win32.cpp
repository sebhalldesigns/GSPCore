#include <GWindow/GWindow.hpp>

#define WIN32_LEAN_AND_MEAN

#ifndef UNICODE
#define UNICODE
#endif 


#ifndef _UNICODE
#define _UNICODE
#endif 

#include <Windows.h>
#include <GL/gl.h>
#include <cstdio>
#include <string>

#define WGL_CONTEXT_MAJOR_VERSION_ARB             0x2091
#define WGL_CONTEXT_MINOR_VERSION_ARB             0x2092
#define WGL_CONTEXT_PROFILE_MASK_ARB              0x9126

#define WGL_CONTEXT_CORE_PROFILE_BIT_ARB          0x00000001

#define WGL_DRAW_TO_WINDOW_ARB                    0x2001
#define WGL_ACCELERATION_ARB                      0x2003
#define WGL_SUPPORT_OPENGL_ARB                    0x2010
#define WGL_DOUBLE_BUFFER_ARB                     0x2011
#define WGL_PIXEL_TYPE_ARB                        0x2013
#define WGL_COLOR_BITS_ARB                        0x2014
#define WGL_DEPTH_BITS_ARB                        0x2022
#define WGL_STENCIL_BITS_ARB                      0x2023

#define WGL_FULL_ACCELERATION_ARB                 0x2027
#define WGL_TYPE_RGBA_ARB                         0x202B

typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext,
        const int *attribList);
wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;

typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int *piAttribIList,
        const FLOAT *pfAttribFList, UINT nMaxFormats, int *piFormats, UINT *nNumFormats);
wglChoosePixelFormatARB_type *wglChoosePixelFormatARB;

// LOCAL FUNCTIONS

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

// function to convert utf8 std::string to std::wstring
std::wstring utf8_to_wstring(const std::string& str);

// STATIC VARIABLES

const static LPCWSTR INIT_WINDOW_CLASS = L"OpenGLWindowClass";
const static LPCWSTR RUNTIME_WINDOW_CLASS = L"GSPCoreWindowClass";

WNDCLASSW init_window_class = { };
WNDCLASSW runtime_window_class = { };


bool GWindow::Init()
{

    init_window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    init_window_class.lpfnWndProc = DefWindowProcW;
    init_window_class.hInstance = GetModuleHandle(0);
    init_window_class.lpszClassName = INIT_WINDOW_CLASS;

    runtime_window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    runtime_window_class.lpfnWndProc = WindowProc;
    runtime_window_class.hInstance = GetModuleHandle(nullptr);
    runtime_window_class.hCursor = LoadCursor(NULL, IDC_ARROW);
    runtime_window_class.hbrBackground = (HBRUSH) (COLOR_WINDOW + 1);
    runtime_window_class.lpszClassName = RUNTIME_WINDOW_CLASS;

    if (!RegisterClassW(&init_window_class) || !RegisterClassW(&runtime_window_class))
    {
        return false;
    }

    HWND dummy_window = CreateWindowW(
        init_window_class.lpszClassName,
        L"Dummy OpenGL Window",
        0,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        0,
        0,
        init_window_class.hInstance,
        0);

    if (!dummy_window) 
    {
        return false;
    }

    HDC dummy_dc = GetDC(dummy_window);

    PIXELFORMATDESCRIPTOR pfd = {};
    pfd.nSize = sizeof(pfd);
    pfd.nVersion = 1;
    pfd.iPixelType = PFD_TYPE_RGBA;
    pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.cColorBits = 32;
    pfd.cAlphaBits = 8;
    pfd.iLayerType = PFD_MAIN_PLANE;
    pfd.cDepthBits = 24;
    pfd.cStencilBits = 8;

    int pixel_format = ChoosePixelFormat(dummy_dc, &pfd);
    if (!pixel_format) 
    {
        return false;
    }

    if (!SetPixelFormat(dummy_dc, pixel_format, &pfd))
    {
        return false;
    }

    HGLRC dummy_context = wglCreateContext(dummy_dc);
    
    if (!dummy_context) 
    {
        return false;
    }

    if (!wglMakeCurrent(dummy_dc, dummy_context)) 
    {
        return false;
    }

    wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)wglGetProcAddress(
        "wglCreateContextAttribsARB");
    wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)wglGetProcAddress(
        "wglChoosePixelFormatARB");

    wglMakeCurrent(dummy_dc, 0);
    wglDeleteContext(dummy_context);
    ReleaseDC(dummy_window, dummy_dc);
    DestroyWindow(dummy_window);

    return true;
}

void GWindow::Poll() {
    // Generally you'll want to empty out the message queue before each rendering
    // frame or messages will build up in the queue possibly causing input
    // delay. Multiple messages and input events occur before each frame.

    MSG msg = { };

    while (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE)) {
        TranslateMessage(&msg);
        DispatchMessageW(&msg);

        if (msg.message == WM_QUIT || (msg.message == WM_KEYDOWN && msg.wParam == VK_ESCAPE))
            exit(EXIT_SUCCESS);
    }
}

GWindow::GWindow(const char* title, float width, float height) 
{   
    HWND hwnd = CreateWindowW(
        RUNTIME_WINDOW_CLASS,
        utf8_to_wstring(title).c_str(),
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, (int)width, (int)height,
        NULL,
        NULL,
        GetModuleHandle(nullptr),
        NULL
    );

    if (hwnd == NULL) {
        return;
    }

    HDC hdc = GetDC(hwnd);
    
    // Now we can choose a pixel format the modern way, using wglChoosePixelFormatARB.
    int pixel_format_attribs[] = {
        WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
        WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
        WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
        WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,         32,
        WGL_DEPTH_BITS_ARB,         24,
        WGL_STENCIL_BITS_ARB,       8,
        0
    };

    int pixel_format;
    UINT num_formats;
    wglChoosePixelFormatARB(hdc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
    if (!num_formats) {
        printf("Failed to set the OpenGL 3.3 pixel format.\n");
        exit(EXIT_FAILURE);
    }

    PIXELFORMATDESCRIPTOR pfd;
    DescribePixelFormat(hdc, pixel_format, sizeof(pfd), &pfd);
    if (!SetPixelFormat(hdc, pixel_format, &pfd)) {
        printf("Failed to set the OpenGL 3.3 pixel format.\n");
        exit(EXIT_FAILURE);
    }

    // Specify that we want to create an OpenGL 3.3 core profile context
    int gl33_attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0,
    };

    HGLRC gl33_context = wglCreateContextAttribsARB(hdc, 0, gl33_attribs);
    if (!gl33_context) {
        printf("Failed to create OpenGL 3.3 context.\n");
        exit(EXIT_FAILURE);
    }

    if (!wglMakeCurrent(hdc, gl33_context)) {
        printf("Failed to activate OpenGL 3.3 rendering context.\n");
        exit(EXIT_FAILURE);
    }

    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) 
{
    switch (uMsg) {
        case WM_SIZE:
            // Handle window resize
            glViewport(0, 0, LOWORD(lParam), HIWORD(lParam));
            PostMessage(hwnd, WM_PAINT, 0, 0);
            return 0;
        case WM_PAINT:

            printf("PAINT\n");
            // Render the scene
            glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            // Add your rendering code here
            SwapBuffers(GetDC(hwnd));
            ValidateRect(hwnd, NULL);
            return 0;
        case WM_CLOSE:
            PostQuitMessage(0);
            return 0;
        default:
            return DefWindowProcW(hwnd, uMsg, wParam, lParam);
    }

   return DefWindowProcW(hwnd, uMsg, wParam, lParam);
}

std::wstring utf8_to_wstring(const std::string& str) {
    int count = MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), NULL, 0);
    std::wstring wstr(count, 0);
    MultiByteToWideChar(CP_UTF8, 0, str.c_str(), str.length(), &wstr[0], count);
    return wstr;
}