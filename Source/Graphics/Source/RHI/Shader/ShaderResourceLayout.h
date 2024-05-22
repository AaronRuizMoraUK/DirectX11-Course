#pragma once

#include <RHI/Resource/Buffer/BufferEnums.h>
#include <RHI/Resource/Texture/TextureEnums.h>

#include <string>
#include <vector>

namespace DX
{
    struct ShaderResourceInfo
    {
        ShaderResourceInfo(
            const std::string& name, uint32_t startSlot, uint32_t slotCount)
            : m_name(name)
            , m_startSlot(startSlot)
            , m_slotCount(slotCount)
        {
        }

        ShaderResourceInfo(
            const std::string& name, uint32_t startSlot, uint32_t slotCount, BufferType bufferType)
            : m_name(name)
            , m_startSlot(startSlot)
            , m_slotCount(slotCount)
            , m_bufferType(bufferType)
        {
        }

        ShaderResourceInfo(
            const std::string& name, uint32_t startSlot, uint32_t slotCount,
            TextureType textureType, TextureSubTypeFlags textureSubTypeFlags = 0)
            : m_name(name)
            , m_startSlot(startSlot)
            , m_slotCount(slotCount)
            , m_textureType(textureType)
            , m_textureSubTypeFlags(textureSubTypeFlags)
        {
        }

        std::string m_name;
        uint32_t m_startSlot = 0;
        uint32_t m_slotCount = 0;

        // For Buffer View only
        BufferType m_bufferType = BufferType::None;

        // For Texture View only
        TextureType m_textureType = TextureType::Unknown;
        TextureSubTypeFlags m_textureSubTypeFlags = 0;
    };

    struct ShaderResourceLayout
    {
        std::vector<ShaderResourceInfo> m_constantBuffers;
        std::vector<ShaderResourceInfo> m_shaderResourceViews; // Shader resource view can hold textures or buffers
        std::vector<ShaderResourceInfo> m_shaderRWResourceViews; // Shader RW resource view can hold textures or buffers
        std::vector<ShaderResourceInfo> m_samplers;
    };
} // namespace DX
