#include "Core.h"

#include <stdio.h>

#include "LoggerFactory.h"
#include "Window.h"

namespace Core {

void init() { Window::get().init(); }

}  // namespace Core