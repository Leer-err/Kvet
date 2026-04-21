#include "InternalRenderTarget.h"

#include "GraphicsResources.h"

namespace Graphics::Internal {

void RenderTarget::destroy() {
    vkDestroyImageView(Resources::get().getDevice(), render_target, nullptr);

    render_target = VK_NULL_HANDLE;
    format = VK_FORMAT_UNDEFINED;
}

}  // namespace Graphics::Internal