#ifndef X11WINDOW_HPP
#define X11WINDOW_HPP

#include <GWindow/GWindowBase.hpp>
#include <GViewCompositor/OpenGL/OpenGLViewCompositor.hpp>

#include <X11/Xlib.h>
#include <X11/Xutil.h>


class X11Window: public GWindowBase
{
    private:

        Window window = 0;
        OpenGLViewCompositor* compositor = nullptr;

        X11Window() = default;
        ~X11Window() = default;

        // returns exit code
        static int RunXLoop();

    public:
        
        static X11Window* Create(std::string title, GSize size);
        static void Destroy(X11Window* window);

        void SetDelegate(GWindowDelegate* delegate) final;
        void SetRootView(GView* view) final;
        void SetTitle(std::string title) final;
        void SetSize(GSize size) final;
        void SetBackgroundColor(GColor color) final;
        void SetVisibility(GWindowVisibility visibility) final;
        void SetMaximumSize(GSize size) final;
        void SetMinimumSize(GSize size) final;

        GWindowDelegate* GetDelegate() final;
        GView* GetRootView() final;
        std::string GetTitle() final;
        GSize GetSize() final;
        GColor GetBackgroundColor() final;
        GWindowVisibility GetVisibility() final;
        GSize GetMaximumSize() final;
        GSize GetMinimumSize() final;

    friend class GApplication;;
};

#endif // X11WINDOW_HPP