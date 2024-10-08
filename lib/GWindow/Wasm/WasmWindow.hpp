#ifndef WASMWINDOW_HPP
#define WASMWINDOW_HPP



#include <GWindow/GWindowBase.hpp>

#include <GViewCompositor/WebGL/WebGLViewCompositor.hpp>

class WasmWindow: public GWindowBase
{
    private:

        WebGLViewCompositor* compositor;

        WasmWindow() = default;
        ~WasmWindow() = default;

        static EM_BOOL EmscriptenResizeCallback(int eventType, const EmscriptenUiEvent *uiEvent, void *userData);
    public:
        
        static WasmWindow* Create(std::string title, GSize size);
        static void Destroy(WasmWindow* window);

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
};


#endif // WASMWINDOW_HPP