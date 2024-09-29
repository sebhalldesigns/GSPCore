#include <GViewCompositor/Direct3D/Direct3DViewCompositor.hpp>

struct Vertex {
    DirectX::XMFLOAT3 position;
    DirectX::XMFLOAT4 color;
};

// Vertex shader source code
const char* vertexShaderSource = R"(
cbuffer ConstantBuffer : register(b0) {
    float4x4 transformation;
};

struct VS_INPUT {
    float3 position : POSITION;
    float4 color : COLOR;
};

struct PS_INPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

PS_INPUT main(VS_INPUT input) {
    PS_INPUT output;
    output.position = mul(float4(input.position, 1.0), transformation);
    output.color = input.color;
    return output;
}
)";

// Pixel shader source code
const char* pixelShaderSource = R"(
struct PS_INPUT {
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

float4 main(PS_INPUT input) : SV_Target {
    return input.color;
}
)";

Direct3DViewCompositor* Direct3DViewCompositor::Create(HWND window)
{   

    Direct3DViewCompositor* compositor = new Direct3DViewCompositor();
    compositor->hwnd = window;

    DXGI_SWAP_CHAIN_DESC scd = {};
    scd.BufferCount = 1;
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    scd.OutputWindow = window;
    scd.SampleDesc.Count = 1;
    scd.Windowed = TRUE;
    scd.BufferDesc.Width = 800;
    scd.BufferDesc.Height = 600;
    scd.BufferDesc.RefreshRate.Numerator = 60;
    scd.BufferDesc.RefreshRate.Denominator = 1;

    D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0,
                                  D3D11_SDK_VERSION, &scd, &compositor->swapChain, &compositor->device, nullptr, &compositor->deviceContext);

    ID3D11Texture2D* backBuffer;
    compositor->swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer));
    compositor->device->CreateRenderTargetView(backBuffer, nullptr, &compositor->renderTargetView);
    backBuffer->Release();

    compositor->deviceContext->OMSetRenderTargets(1, & compositor->renderTargetView, nullptr);

    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = 800;
    viewport.Height = 600;
    compositor->deviceContext->RSSetViewports(1, &viewport); 

    // INIT PIPELINE

    ID3DBlob* vsBlob = nullptr;
    ID3DBlob* psBlob = nullptr;
    D3DCompile(vertexShaderSource, strlen(vertexShaderSource), nullptr, nullptr, nullptr, "main", "vs_5_0", 0, 0, &vsBlob, nullptr);
    D3DCompile(pixelShaderSource, strlen(pixelShaderSource), nullptr, nullptr, nullptr, "main", "ps_5_0", 0, 0, &psBlob, nullptr);

    compositor->device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), nullptr, &compositor->vertexShader);
    compositor->device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), nullptr, &compositor->pixelShader);

    compositor->deviceContext->VSSetShader(compositor->vertexShader, nullptr, 0);
    compositor->deviceContext->PSSetShader(compositor->pixelShader, nullptr, 0);

    D3D11_INPUT_ELEMENT_DESC layout[] = {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    compositor->device->CreateInputLayout(layout, 2, vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), &compositor->inputLayout);
    compositor->deviceContext->IASetInputLayout(compositor->inputLayout);

    vsBlob->Release();
    psBlob->Release();

    // init graphics

    Vertex vertices[] = {
        {{0.0f, 0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
    };

    D3D11_BUFFER_DESC bufferDesc = {};
    bufferDesc.Usage = D3D11_USAGE_DEFAULT;
    bufferDesc.ByteWidth = sizeof(vertices);
    bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

    D3D11_SUBRESOURCE_DATA initData = {};
    initData.pSysMem = vertices;

    compositor->device->CreateBuffer(&bufferDesc, &initData, &compositor->vertexBuffer);

    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    compositor->deviceContext->IASetVertexBuffers(0, 1, &compositor->vertexBuffer, &stride, &offset);
    compositor->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    return compositor;
}

void Direct3DViewCompositor::Destroy(Direct3DViewCompositor* compositor) {

    compositor->vertexBuffer->Release();
    compositor->inputLayout->Release();
    compositor->vertexShader->Release();
    compositor->pixelShader->Release();
    compositor->renderTargetView->Release();
    compositor->swapChain->Release();
    compositor->deviceContext->Release();
    compositor->device->Release();

    delete compositor;
}

void Direct3DViewCompositor::Resize(GSize size)
{
    swapChain->ResizeBuffers(1, size.Width, size.Height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = size.Width;
    viewport.Height = size.Height;
    deviceContext->RSSetViewports(1, &viewport);
}

void Direct3DViewCompositor::Render()
{
    float clearColor[] = {0.0f, 0.2f, 0.4f, 1.0f};
    deviceContext->ClearRenderTargetView(renderTargetView, clearColor);
    deviceContext->Draw(3, 0);
    swapChain->Present(1, 0);
}