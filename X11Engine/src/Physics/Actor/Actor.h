#pragma once

#include "Shape.h"

namespace physx {
class PxActor;
};

namespace Physics {

class Actor {
   public:
    virtual void addShape(const Shape& shape) = 0;
};

};  // namespace Physics