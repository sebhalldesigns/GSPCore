#ifndef WIN32WINDOW_HPP
#define WIN32WINDOW_HPP

#include <GWindow/GWindowBase.hpp>

#include <Windows.h>



class Win32Window: public GWindowBase
{
    private:
        HWND hwnd;
        //HGLRC glContext;

        Win32Window() = default;
        ~Win32Window() = default;

        static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    public:
        
        static Win32Window* Create(std::string title, GSize size);
        static void Destroy(Win32Window* window);

        void SetDelegate(GWindowDelegate* delegate) final;
        void SetTitle(std::string title) final;
        void SetSize(GSize size) final;
        void SetBackgroundColor(GColor color) final;
        void SetVisibility(GWindowVisibility visibility) final;
        void SetMaximumSize(GSize size) final;
        void SetMinimumSize(GSize size) final;

        GWindowDelegate* GetDelegate() final;
        std::string GetTitle() final;
        GSize GetSize() final;
        GColor GetBackgroundColor() final;
        GWindowVisibility GetVisibility() final;
        GSize GetMaximumSize() final;
        GSize GetMinimumSize() final;
};

#endif // WIN32WINDOW_HPP