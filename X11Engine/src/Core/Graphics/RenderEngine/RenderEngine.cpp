#include <memory>

#include "RenderEngine.h"
#include "RenderEngineImpl.h"


namespace Graphics {

RenderEngine::RenderEngine()
    : impl(std::move(std::make_unique<RenderEngineImpl>())) {}

RenderEngine::~RenderEngine() = default;

void RenderEngine::init() { impl->init(); }

void RenderEngine::render() { impl->render(); }

uint32_t RenderEngine::getWidth() const { return impl->getWidth(); }

uint32_t RenderEngine::getHeight() const { return impl->getHeight(); }

}  // namespace Graphics