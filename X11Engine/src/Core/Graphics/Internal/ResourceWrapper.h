#pragma once

#include <vulkan/vulkan_core.h>

namespace Graphics::Internal {

template <typename T>
class ResourceWrapper {
   public:
    ResourceWrapper(const T& resource, bool should_be_destroyed = true)
        : resource(resource), should_be_destroyed(should_be_destroyed) {}
    ~ResourceWrapper() {
        if (should_be_destroyed) resource.destroy();
    }

    T get() const { return resource; }
    T& getRef() { return resource; }
    T* getPtr() { return &resource; }

   private:
    T resource;
    bool should_be_destroyed;
};

}  // namespace Graphics::Internal