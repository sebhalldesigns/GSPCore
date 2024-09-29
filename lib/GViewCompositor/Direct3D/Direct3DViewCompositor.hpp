#ifndef DIRECT3DVIEWCOMPOSITOR_HPP
#define DIRECT3DVIEWCOMPOSITOR_HPP


#include <GViewCompositor/GViewCompositorBase.hpp>

#include <GSize.hpp>

#include <windows.h>
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>



class Direct3DViewCompositor: public GViewCompositorBase
{
    private:

        HWND hwnd;

        ID3D11Device* device;
        ID3D11DeviceContext* deviceContext;
        IDXGISwapChain* swapChain;
        ID3D11RenderTargetView* renderTargetView;
        ID3D11InputLayout* inputLayout;
        ID3D11Buffer* vertexBuffer;
        ID3D11VertexShader* vertexShader;
        ID3D11PixelShader* pixelShader;

        void Resize(GSize size) final;
        void Render(GView* view) final;

        static Direct3DViewCompositor* Create(HWND window);
        static void Destroy(Direct3DViewCompositor* compositor);

    friend class Win32Window;
    
};

#endif // DIRECT3DVIEWCOMPOSITOR_HPP