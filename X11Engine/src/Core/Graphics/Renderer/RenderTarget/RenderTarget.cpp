#include "RenderTarget.h"

#include <memory>

#include "InternalRenderTarget.h"
#include "ResourceWrapper.h"

namespace Graphics {

RenderTarget::RenderTarget(const Internal::RenderTarget& render_target)
    : render_target(
          std::make_shared<Internal::WrappedRenderTarget>(render_target)) {}

Internal::RenderTarget* RenderTarget::getInternal() const {
    return render_target->getPtr();
}

}  // namespace Graphics