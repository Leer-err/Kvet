#pragma once

#include <map>
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
        channel.send(data);
    }

    template <typename T>
    std::optional<T> recieve() {
        auto channel = getChannel<T>();
        if (channel == nullptr) return {};

        return channel.recieve();
    }

   private:
    template <typename T>
    Channel<T>* getChannel() {
        auto type_id = TypeIdHelper::getTypeId<T>();
        auto channel_it = channels.find(type_id);
        if (channel_it == channels.end()) return nullptr;

        return &channel_it->second;
    }

    template <typename T>
    Channel<T>* getOrCreteChannel() {
        auto ptr = getChannel<T>();
        if (ptr != nullptr) return ptr;

        auto type_id = TypeIdHelper::getTypeId<T>();
        channels.emplace(type_id, Channel<T>());

        return &channels[type_id];
    }

    std::map<TypeId, IChannel*> channels;
};