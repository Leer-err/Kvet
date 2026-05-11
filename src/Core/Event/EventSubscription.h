#ifndef EVENT_SUBSCRIPTION_H
#define EVENT_SUBSCRIPTION_H

#include <functional>
#include <optional>

namespace Event {

class EventSubscription {
   public:
    EventSubscription();
    EventSubscription(const std::function<void()>& unsubscribe_callback);
    ~EventSubscription() noexcept;

    EventSubscription(EventSubscription&&);
    EventSubscription& operator=(EventSubscription&&);

   private:
    std::optional<std::function<void()>> unsubscribe_callback;
};

};  // namespace Event

#endif  // EVENT_SUBSCRIPTION_H