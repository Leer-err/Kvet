#pragma once

#include <map>
#include <memory>
#include <optional>

#include "Channel.h"
#include "TypeId.h"
#include "TypeIdHelper.h"

class GraphicsCommunicationManager {
   public:
    static GraphicsCommunicationManager& get() {
        static GraphicsCommunicationManager instance;
        return instance;
    }

    template <typename T>
    void send(const T& data) {
        auto channel = getOrCreteChannel<T>();
        channel->send(data);
    }

    template <typename T>
    std::optional<T> recieve() {
        auto channel = getChannel<T>();
        if (channel == nullptr) return {};

        return channel->recieve();
    }

    void flush() {
        for (const auto& [type_id, channel] : channels) {
            channel->flush();
        }
    }

   private:
    template <typename T>
    Channel<T>* getChannel() {
        auto type_id = TypeIdHelper::getTypeId<T>();
        auto channel_it = channels.find(type_id);
        if (channel_it == channels.end()) return nullptr;

        return reinterpret_cast<Channel<T>*>(channel_it->second.get());
    }

    template <typename T>
    Channel<T>* getOrCreteChannel() {
        auto ptr = getChannel<T>();
        if (ptr != nullptr) return ptr;

        auto type_id = TypeIdHelper::getTypeId<T>();

        channels.emplace(type_id, std::move(std::make_unique<Channel<T>>()));

        return reinterpret_cast<Channel<T>*>(channels[type_id].get());
    }

   private:
    GraphicsCommunicationManager() = default;

    GraphicsCommunicationManager(const GraphicsCommunicationManager&) = delete;
    GraphicsCommunicationManager& operator=(
        const GraphicsCommunicationManager&) = delete;

    std::map<TypeId, std::unique_ptr<IChannel>> channels;
};