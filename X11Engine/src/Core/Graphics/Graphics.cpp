#include "Graphics.h"

#include "ExtensionFunctions.h"
#include "GraphicsResources.h"

namespace Graphics {

void init() {
    Resources::get();
    loadExtensionFunctions();
}

}  // namespace Graphics