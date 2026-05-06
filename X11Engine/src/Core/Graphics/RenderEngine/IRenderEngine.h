#pragma once

namespace Graphics {

class IRenderEngine {
   public:
    virtual void render() = 0;
};

}  // namespace Graphics