#ifndef EVENT_LISTENER_H
#define EVENT_LISTENER_H

#include <functional>

#include "Event.h"

namespace Event {

class EventListener {
   public:
    EventListener(std::function<void(const Event* event)> callback);

    void execute(const Event* event);

   private:
    std::function<void(const Event* event)> callback;
};

};  // namespace Event

#endif  // EVENT_LISTENER_H