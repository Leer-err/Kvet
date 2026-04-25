#pragma once

#include <mutex>
#include <optional>
#include <queue>

class IChannel {
   public:
    virtual ~IChannel() {}

    virtual void flush() = 0;
};

template <typename T>
class Channel : public IChannel {
   public:
    Channel() : ready_elements_count(0) {}

    void send(const T& data) {
        std::scoped_lock<std::mutex> lock(access_mutex);

        queue.push(data);
    }

    std::optional<T> recieve() {
        std::scoped_lock<std::mutex> lock(access_mutex);

        if (ready_elements_count == 0) return {};

        auto value = queue.back();
        queue.pop();

        ready_elements_count--;

        return value;
    }

    void flush() {
        std::scoped_lock<std::mutex> lock(access_mutex);

        ready_elements_count = queue.size();
    }

   private:
    std::queue<T> queue;
    std::mutex access_mutex;

    size_t ready_elements_count;
};