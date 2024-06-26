#pragma once

#include <RHI/Resource/Buffer/BufferEnums.h>
#include <RHI/Resource/Texture/TextureEnums.h>

#include <string>
#include <vector>

namespace DX
{
    struct ShaderResourceInfo
    {
        ShaderResourceInfo() = default;

        ShaderResourceInfo(
            const std::string& name, uint32_t startSlot, uint32_t slotCount)
            : m_name(name)
            , m_startSlot(startSlot)
            , m_slotCount(slotCount)
        {
        }

        ShaderResourceInfo(
            const std::string& name, uint32_t startSlot, uint32_t slotCount, BufferSubType bufferSubType)
            : m_name(name)
            , m_startSlot(startSlot)
            , m_slotCount(slotCount)
            , m_bufferSubType(bufferSubType)
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
        BufferSubType m_bufferSubType = BufferSubType::None;

        // For Texture View only
        TextureType m_textureType = TextureType::Unknown;
        TextureSubTypeFlags m_textureSubTypeFlags = 0;
    };

    // Defines what resources are used by a shader and the slots they are expected to be bound to.
    struct ShaderResourceLayout
    {
        std::vector<ShaderResourceInfo> m_constantBuffers;
        std::vector<ShaderResourceInfo> m_shaderResourceViews; // Shader resource view can hold textures or buffers
        std::vector<ShaderResourceInfo> m_shaderRWResourceViews; // Shader RW resource view can hold textures or buffers
        std::vector<ShaderResourceInfo> m_samplers;

        uint32_t m_constantBuffersSlotCount = 0;
        uint32_t m_shaderResourceViewsSlotCount = 0;
        uint32_t m_shaderRWResourceViewsSlotCount = 0;
        uint32_t m_samplersSlotCount = 0;
    };
} // namespace DX
