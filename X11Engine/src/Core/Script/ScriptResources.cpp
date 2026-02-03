#include "ScriptResources.h"

namespace Script {

sol::state_view Resources::getState() { return state; }

};  // namespace Script