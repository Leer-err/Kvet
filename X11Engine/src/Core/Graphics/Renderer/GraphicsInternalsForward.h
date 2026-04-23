#pragma once

namespace Graphics::Internal {

struct Buffer;
struct Texture;
struct SwapChain;
struct Shader;
struct Pipeline;
struct RenderTarget;
struct CommandBuffer;
struct InputLayout;

template <typename T>
class ResourceWrapper;

using WrappedRenderTarget = ResourceWrapper<RenderTarget>;
using WrappedPipeline = ResourceWrapper<Pipeline>;
using WrappedInputLayout = ResourceWrapper<InputLayout>;
using WrappedTexture = ResourceWrapper<Texture>;

}  // namespace Graphics::Internal