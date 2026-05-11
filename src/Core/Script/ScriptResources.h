#pragma once

#include <sol/sol.hpp>

namespace Script {

class Resources {
   public:
    static Resources& get() {
        static Resources instance;
        return instance;
    }

    sol::state& getState();

   private:
    sol::state state;
};

};  // namespace Script