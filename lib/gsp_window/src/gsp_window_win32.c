#include "gsp_window_win32.h"
#include "gsp_window_internal.h"


#include <gsp_debug/gsp_debug.h>

#include <GL/gl.h>
#include "wglext.h"

#include <stdio.h>
#include <stdlib.h>

static gwindow_win32_state_t state;
const static wchar_t CLASS_NAME[] = L"gsp_window";

typedef struct linked_win32_window_t {
    gwindow_t window;
    struct linked_win32_window_t* next;
    HDC device_context;
    HGLRC gl_context;
} linked_win32_window_t;

static linked_win32_window_t* first_win32_window = NULL;

// implemented at end of file
LRESULT CALLBACK gsp_window_win32_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

gwindow_t gsp_window_win32_create_window() {

    if (state.instance == NULL) {

        gsp_debug_log(INFO, "Starting Win32..");

        state.instance = GetModuleHandle(NULL);
        
        WNDCLASS wc = { 0 };
            wc.lpfnWndProc   = gsp_window_win32_wndproc;
            wc.hInstance     = state.instance;
            wc.lpszClassName = CLASS_NAME;

            if (RegisterClass(&wc) == NULL) {
                DWORD errorCode = GetLastError();

                gsp_debug_log(ERROR, "Failed to register a Win32 class, error code: %lu", errorCode);

                return (gwindow_t) 0;
            }

            gsp_debug_log(INFO, "Started GWin32WindowManager ok");

    }

    HWND win32_window = CreateWindow(
        CLASS_NAME,
        L"Sample Window",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL,
        NULL,
        GetModuleHandle(NULL),
        NULL
    );

    HDC hdc = GetDC(win32_window);

    // Set up a temporary context to get function pointers
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1,
        PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        PFD_TYPE_RGBA,
        32,    // Color depth
        0, 0, 0, 0, 0, 0,
        0,
        0,
        0,
        0, 0, 0, 0,
        24,    // Depth buffer
        8,     // Stencil buffer
        0,
        PFD_MAIN_PLANE,
        0,
        0, 0, 0
    };

    int pixelFormat = ChoosePixelFormat(hdc, &pfd);
    if (pixelFormat == 0) {
        gsp_debug_log(ERROR, "Failed to choose pixel format.");
    }

    if (!SetPixelFormat(hdc, pixelFormat, &pfd)) {
        gsp_debug_log(ERROR, "Failed to set pixel format.");
    }

    HGLRC tempContext = wglCreateContext(hdc);
    if (!tempContext) {
        gsp_debug_log(ERROR, "Failed to create temporary OpenGL context.");
    }

    if (!wglMakeCurrent(hdc, tempContext)) {
        gsp_debug_log(ERROR, "Failed to activate temporary OpenGL context.");
    }

    // Load OpenGL functions using a library like GLEW or manually load function pointers
    // Here, we're loading the wglCreateContextAttribsARB function manually for demonstration

    PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)wglGetProcAddress("wglCreateContextAttribsARB");
    if (!wglCreateContextAttribsARB) {
        gsp_debug_log(ERROR, "Failed to load wglCreateContextAttribsARB.");
    }

    // Specify the desired OpenGL version and context attributes
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 3,
        WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };

    // Create the OpenGL 3.3 context
    HGLRC hglrc = wglCreateContextAttribsARB(hdc, 0, attribs);
    if (!hglrc) {
        gsp_debug_log(ERROR, "Failed to create OpenGL 3.3 context.");
    }

    // Make the new context current and delete the temporary context
    if (!wglMakeCurrent(hdc, hglrc)) {
        gsp_debug_log(ERROR, "Failed to activate OpenGL 3.3 context.");
    }

    wglDeleteContext(tempContext);

    // Release the device context
    ReleaseDC(win32_window, hdc);



    ShowWindow(win32_window, SW_SHOW);

    linked_win32_window_t* new_window = (linked_win32_window_t*) malloc(sizeof(linked_win32_window_t));
        
    if (NULL == new_window) {
        // linked window allocation failed, return an invalid window
        return (gwindow_t) 0;
    }

    new_window->window = win32_window;
    new_window->device_context = hdc;
    new_window->gl_context = hglrc;
    new_window->next = NULL;


    // insert into linked list
    if (NULL == first_win32_window) {

        // set as the first window
        first_win32_window = new_window;

    } else {
        
        // add to the end of the linked list

        linked_win32_window_t* last_window = first_win32_window;

        while (last_window->next != NULL) {
            last_window = last_window->next;
        }

        last_window->next  = new_window;

    }

    return (gwindow_t) win32_window;
}

void gsp_window_win32_poll_events() {
    while (GetMessage(&state.msg, NULL, 0, 0)) {
        TranslateMessage(&state.msg);
        DispatchMessage(&state.msg);
    }
}

void gsp_window_win32_set_title(gwindow_t window, gstring_t title) {
    SetWindowText((HWND)window, title);
}

LRESULT CALLBACK gsp_window_win32_wndproc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {

    if (NULL == first_win32_window) {
        // no valid windows open
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    // iterate over windows linked list

    linked_win32_window_t* last_window = first_win32_window;

    linked_win32_window_t* this_window = NULL;

    while (last_window != NULL) {
        
        if (hwnd == last_window->window) {
            
            this_window = last_window;

            last_window = NULL;
            break;
        } else {
            last_window = last_window->next;
        }

    }

    if (NULL == this_window) {
        // this window is unknown to the system
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    gwindow_event_t event;

    switch (msg) {
        case WM_SIZE:
            float width = (float)LOWORD(lParam);
            float height = (float)HIWORD(lParam);

            uint64_t width_u64 = (uint64_t) width;
            uint64_t height_u64 = (uint64_t) height;
            height_u64 = height_u64 << 32;

            event.event_id = WINDOW_EVENT_RESIZE;
            event.data = width_u64 | height_u64;
            gsp_window_system_event_callback((gwindow_t) hwnd, event);

            HDC hdc = GetDC(hwnd);

            wglMakeCurrent(hdc, this_window->gl_context);
            
            // Set the clear color (RGBA)
            glClearColor(0.0f, 0.0f, 1.0f, 1.0f); // Blue color

            // Clear the color buffer
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap buffers to display the color
            SwapBuffers(hdc);

            break;
        case WM_CLOSE:
            event.event_id = WINDOW_EVENT_CLOSE_REQUEST;
            gsp_window_system_event_callback((gwindow_t) hwnd, event);
            return 0;
    }

    return DefWindowProc(hwnd, msg, wParam, lParam);
}

