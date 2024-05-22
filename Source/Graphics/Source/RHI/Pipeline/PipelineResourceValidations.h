#pragma once

namespace DX
{
    class PipelineResourceBindings;

    // Validate resource binding data with the shaders resource layouts
    void ValidatePipelineResourceBindings(const PipelineResourceBindings& resources);
} // namespace DX
