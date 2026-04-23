#pragma once

#include <stddef.h>

// #include <cstddef>
#include <memory>

#include "GraphicsInternalsForward.h"

namespace Graphics {

class Buffer {
    friend class BufferBuilder;
    friend class Context;

   public:
    Buffer();
    ~Buffer();

    Buffer(Buffer&&);
    Buffer& operator=(Buffer&&);

    size_t getSize() const;

    void* map();
    void unmap();

   protected:
    Buffer(Internal::Buffer&& buffer);
    Internal::Buffer* getInternal() const;

   private:
    std::unique_ptr<Internal::Buffer> buffer;
};

}  // namespace Graphics
