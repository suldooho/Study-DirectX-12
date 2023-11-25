#pragma once
#include "framework.h" 

template <typename T>
class UploadBuffer
{
private:
    ComPtr<ID3D12Resource> m_uploadBuffer;
    BYTE* m_mappedData;

    UINT m_elementByteSize;
    bool m_isConstantBuffer;

public:
    UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer);
    UploadBuffer(const UploadBuffer& rhs) = delete;
    UploadBuffer& operator=(const UploadBuffer& rhs) = delete;
    ~UploadBuffer();

public:
    ID3D12Resource* Resource() const;
    void CopyData(INT elementIndex, const T& data);
};

struct ObjectConstants
{
    XMFLOAT4X4 World = Identity4X4();
};

struct PassConstants
{
    XMFLOAT4X4 View = Identity4X4();
    XMFLOAT4X4 InvView = Identity4X4();
    XMFLOAT4X4 Proj = Identity4X4();
    XMFLOAT4X4 InvProj = Identity4X4();
    XMFLOAT4X4 ViewProj = Identity4X4();
    XMFLOAT4X4 InvViewProj = Identity4X4();
    XMFLOAT3 EyePosW = { 0.0f, 0.0f, 0.0f };
    float cbPerObjectPad1 = 0.0f;
    XMFLOAT2 RenderTargetSize = { 0.0f, 0.0f };
    XMFLOAT2 InvRenderTargetSize = { 0.0f, 0.0f };
    float NearZ = 0.0f;
    float FarZ = 0.0f;
    float TotalTime = 0.0f;
    float DeltaTime = 0.0f;
};

struct Vertex
{
    XMFLOAT3 Pos;
    XMFLOAT4 Color;
};

struct FrameResource
{ 
    FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount);
    FrameResource(const FrameResource& rhs) = delete;
    FrameResource& operator=(const FrameResource& rhs) = delete;
    ~FrameResource();

    ComPtr<ID3D12CommandAllocator> commandAllocator;

    unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
    //unique_ptr<UploadBuffer<PassConstants>> PassCB = nullptr;
    //unique_ptr<UploadBuffer<ObjectConstants>> ObjectCB = nullptr;

    UINT64 Fence = 0;
};