#pragma once

#include <RHI/Shader/ShaderBytecode.h>

#include <RHI/DirectX/ComPtr.h>
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;

namespace DX
{
    struct ShaderResourceLayout;

    class DX11ShaderBytecode : public ShaderBytecode
    {
    public:
        DX11ShaderBytecode(ComPtr<ID3DBlob>&& dx11Blob, ShaderResourceLayout&& resourceLayout);
        ~DX11ShaderBytecode() = default;

        DX11ShaderBytecode(const DX11ShaderBytecode&) = delete;
        DX11ShaderBytecode& operator=(const DX11ShaderBytecode&) = delete;

        const void* GetData() const override;
        uint32_t GetSize() const override;

    private:
        ComPtr<ID3DBlob> m_dx11Blob;
    };
} // namespace DX
