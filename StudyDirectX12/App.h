#pragma once
#include "framework.h" 

class App
{
private:
	static const UINT FrameCount = 2;

private:
	HINSTANCE m_hinstance;
	HWND m_hwnd;

	UINT m_windowWidth;
	UINT m_windowHeight;

private:
	ComPtr <ID3D12Device> m_device;
	ComPtr<IDXGISwapChain3> m_swapChain;
	UINT m_frameIndex;

	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	ComPtr<ID3D12Resource> m_renderTargetBuffers[FrameCount];
	ComPtr<ID3D12DescriptorHeap> m_rtvDescriptorHeap;
	UINT m_rtvDescriptorIncrementSize;

	ComPtr<ID3D12Resource> m_depthStencilBuffer;
	ComPtr<ID3D12DescriptorHeap> m_dsvDescriptorHeap;
	UINT m_dsvDescriptorIncrementSize;

	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValues[FrameCount];
	HANDLE m_fenceEvent;

	D3D12_VIEWPORT m_viewport;
	D3D12_RECT m_scissorRect;

	bool m_tearingSupport;

#if defined(_DEBUG) 
	ComPtr<ID3D12Debug> m_debugController;
	DWORD dxgiFactoryFlags;
#endif

public:
	App();

public:
	void OnCreate(HINSTANCE hinstance, HWND mainWnd);

private:
	void CreateDirect3DDevice();
	void CreateSwapChain();
	void CreateRtvAndDsvDescriptorHeaps();
	void CreateCommandQueueAndList();

	void CreateRenderTargetView();
	void CreateDepthStencilView();
	void CreateFence();
	void CreateViewportAndScissorRect();

	void BuildObjects();
	void ReleaseObjects();

private:  
	void GetHardwareAdapter(IDXGIFactory1* pFactory, IDXGIAdapter1** ppAdapter);
	void CheckTearingSupport();

	void ProcessInput();
	void AnimateObjects();
	void WaitForGpuComplete();

public:
	void FrameAdvance();
};

