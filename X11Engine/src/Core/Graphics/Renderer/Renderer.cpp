#include "Renderer.h"

#include <memory>

#include "RendererImpl.h"

namespace Graphics {

Renderer::Renderer() : impl(std::move(std::make_unique<RendererImpl>())) {}

Renderer::~Renderer() = default;

void Renderer::init() { impl->init(); }

void Renderer::render() { impl->render(); }

uint32_t Renderer::getWidth() const { return impl->getWidth(); }

uint32_t Renderer::getHeight() const { return impl->getHeight(); }

}  // namespace Graphics