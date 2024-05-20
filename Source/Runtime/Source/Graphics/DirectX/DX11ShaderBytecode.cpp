#include <Graphics/DirectX/DX11ShaderBytecode.h>

#include <Log/Log.h>

#include <memory>

#include <d3d11.h>

namespace DX
{
    DX11ShaderBytecode::DX11ShaderBytecode(ComPtr<ID3DBlob>&& dx11Blob)
        : m_dx11Blob(std::move(dx11Blob))
    {
        DX_ASSERT(m_dx11Blob.Get() != nullptr, "DX11ShaderBytecode", "Invalid DX11 blob");
        DX_ASSERT(m_dx11Blob.Get()->GetBufferPointer() != nullptr, "DX11ShaderBytecode", "DX11 Blob has invalid data");
    }

    const void* DX11ShaderBytecode::GetData() const
    {
        return m_dx11Blob->GetBufferPointer();
    }

    uint32_t DX11ShaderBytecode::GetSize() const
    {
        return m_dx11Blob->GetBufferSize();
    }
} // namespace DX
