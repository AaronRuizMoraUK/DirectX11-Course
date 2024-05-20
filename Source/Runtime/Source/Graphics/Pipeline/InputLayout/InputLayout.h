#pragma once

#include <Graphics/Pipeline/InputLayout/InputLayoutEnums.h>
#include <Graphics/Resource/ResourceEnums.h>

#include <vector>

namespace DX
{
    // Describes the layout of a vertex buffer.
    // 
    // Examples of most common input layouts:
    //
    // A) Using one buffer with all vertex elements interlaced.
    // 
    //    Vertex Buffer = [pos0, color0, pos1, color1, ..., posN, colorN]
    // 
    //    InputLayout layout = [
    //        { "POSITION", 0, ResourceFormat::R32G32B32_FLOAT, 0, 0, 0 }, // Input Slot 0, alignedByteOffset 0
    //        { "COLOR", 0, ResourceFormat::R32G32B32A32_FLOAT, 0, 12, 0 } // Input Slot 0, alignedByteOffset 12
    //    ];
    // 
    //    const uint32_t stride = sizeof(Vertex); // vector3 (pos) + vector4 (color)
    //    const uint32_t offset = 0;
    //    deviceContext->IASetVertexBuffers(0, 1, vertexBuffer, &stride, &offset); // Input Slot 0
    // 
    // B) Using multiple buffers, one buffer per vertex element. 
    //
    //    Position Buffer = [pos0, pos1, ..., posN]
    //    Color Buffer = [color0, color1, ..., colorN]
    //
    //    InputLayout layout = [
    //        { "POSITION", 0, ResourceFormat::R32G32B32_FLOAT, 0, 0, 0 }, // Input Slot 0, alignedByteOffset 0
    //        { "COLOR", 0, ResourceFormat::R32G32B32A32_FLOAT, 1, 0, 0 }  // Input Slot 1, alignedByteOffset 0
    //    ];
    // 
    //    const uint32_t stridePos = sizeof(vector3); // pos
    //    const uint32_t offsetPos = 0;
    //    deviceContext->IASetVertexBuffers(0, 1, posBuffer, &stridePos, &offsetPos); // Input Slot 0
    // 
    //    const uint32_t strideColor = sizeof(vector4); // color
    //    const uint32_t offsetColor = 0;
    //    deviceContext->IASetVertexBuffers(1, 1, colorBuffer, &strideColor, &offsetColor); // Input Slot 1

    struct InputElement
    {
        InputSemantic m_semantic;
        std::string m_semanticCustomName; // When using InputSemanticName::CustomName
        uint32_t m_semanticIndex;
        ResourceFormat m_format;
        uint32_t m_inputSlot; // [0, 15]
        uint32_t m_alignedByteOffset; // Offset from the start of the buffer where this element starts
        uint32_t m_instanceDataStepRate;
    };

    struct InputLayout
    {
        std::vector<InputElement> m_inputElements;

        PrimitiveTopology m_primitiveTopology;
        uint32_t m_controlPointPatchListCount; // [1, 32] when using PrimitiveTopology::ControlPointPatchList
    };

} // namespace DX
