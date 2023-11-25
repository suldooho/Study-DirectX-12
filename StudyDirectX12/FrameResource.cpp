#include "FrameResource.h"

template <typename T>
UploadBuffer<T>::UploadBuffer(ID3D12Device* device, UINT elementCount, bool isConstantBuffer) : m_isConstantBuffer(isConstantBuffer)
{
	if (m_isConstantBuffer)
	{
		m_elementByteSize = CalcConstantBufferByteSize(sizeof(T));
	}
	else
	{
		m_elementByteSize = sizeof(T);
	}


	D3D12_HEAP_PROPERTIES heapProperties = {};
	heapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_UNKNOWN;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_UNKNOWN;
	heapProperties.CreationNodeMask = 1;
	heapProperties.VisibleNodeMask = 1;

	D3D12_RESOURCE_DESC resourceDesc = {};
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resourceDesc.Alignment = 0;
	resourceDesc.Width = m_elementByteSize * elementCount;
	resourceDesc.Height = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.MipLevels = 1;
	resourceDesc.Format = DXGI_FORMAT_UNKNOWN;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.SampleDesc.Quality = 0;
	resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_NONE;

	ThrowIfFailed(device->CreateCommittedResource(heapProperties, D3D12_HEAP_FLAG_NONE, resourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&m_uploadBuffer)));

	ThrowIfFailed(m_uploadBuffer->Map(0, nullptr, reinterpret_cast<void**>(&m_mappedData)));
}

template <typename T>
UploadBuffer<T>::~UploadBuffer()
{
	if (m_uploadBuffer != nullptr)
	{
		m_uploadBuffer->Unmap(0, nullptr);
	}

	m_mappedData = nullptr;
}

template <typename T>
ID3D12Resource* UploadBuffer<T>::Resource() const
{
	return m_uploadBuffer.Get();
}

template <typename T>
void UploadBuffer<T>::CopyData(INT elementIndex, const T& data)
{
	memcpy(&m_mappedData[elementIndex * m_elementByteSize], &data, sizeof(T));
}

FrameResource::FrameResource(ID3D12Device* device, UINT passCount, UINT objectCount)
{
	ThrowIfFailed(device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(commandAllocator.GetAddressOf())));

	//PassCB = make_unique<UploadBuffer<PassConstants>>(device, passCount, true);
	//ObjectCB = make_unique<UploadBuffer<ObjectConstants>>(device, objectCount, true);
}

FrameResource::~FrameResource()
{
}
