#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

template <typename T>
class ResourceWrapper {
   public:
    ResourceWrapper(const T& resource) : resource(resource) {}
    ~ResourceWrapper() { resource.destroy(); }

    T get() const { return resource; }
    T& getRef() const { return resource; }
    T* getPtr() { return &resource; }

   private:
    T resource;
};

}  // namespace Graphics::Internal