#include "ScriptResources.h"

namespace Script {

sol::state& Resources::getState() { return state; }

};  // namespace Script