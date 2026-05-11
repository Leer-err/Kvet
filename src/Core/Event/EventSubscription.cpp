#include "EventSubscription.h"

#include <optional>

namespace Event {

EventSubscription::EventSubscription() : unsubscribe_callback(std::nullopt) {}

EventSubscription::EventSubscription(
    const std::function<void()>& unsubscribe_callback)
    : unsubscribe_callback(unsubscribe_callback) {}

EventSubscription::~EventSubscription() noexcept {
    try {
        if (unsubscribe_callback) (*unsubscribe_callback)();
    } catch (...) {
    }
}

EventSubscription::EventSubscription(EventSubscription&& other) {
    std::swap(unsubscribe_callback, other.unsubscribe_callback);
}

EventSubscription& EventSubscription::operator=(EventSubscription&& other) {
    std::swap(unsubscribe_callback, other.unsubscribe_callback);

    return *this;
}

};  // namespace Event