#include "GraphicsPipeline.h"

#include <memory>

#include "Pipeline.h"
#include "ResourceWrapper.h"

namespace Graphics {

GraphicsPipeline::GraphicsPipeline(const Internal::Pipeline& pipeline) {
    this->pipeline = std::make_shared<Internal::WrappedPipeline>(pipeline);
}

const Internal::Pipeline* GraphicsPipeline::getInternal() const {
    return pipeline->getPtr();
}

}  // namespace Graphics