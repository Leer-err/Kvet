#pragma once

#include <bit>
#include <cassert>
#include <cstdint>
#include <functional>
#include <optional>
#include <string>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Handle.h"
#include "PoolAllocator.h"
#include "ResourceRegistryBase.h"
#include "TransparentStringHash.h"

template <typename T>
class ResourceRegistry final : public ResourceRegistryBase {
   public:
    explicit ResourceRegistry(PoolAllocator& allocator)
        : ResourceRegistryBase(allocator) {}
    ~ResourceRegistry() {
        for (auto& [ptr, counter] : ref_counters) remove(ptr);
    }

    template <typename... ARGS>
    Handle<T> create(ARGS&&... args) {
        auto memory = allocate();
        if (memory == nullptr) return Handle<T>();

        return Handle(new (memory) T(std::forward<ARGS>(args)...), this);
    }

    template <auto Func, typename... ARGS>
    Handle<T> create(ARGS&&... args) {
        auto memory = allocate();
        if (memory == nullptr) return Handle<T>();

        return Handle(new (memory) T(std::forward<ARGS>(args)...), this);
    }

    std::vector<Handle<T>> getAllocatedObjects() {
        auto objects = std::vector<Handle<T>>(ref_counters.size());

        int index = 0;
        for (auto& [key, value] : ref_counters) {
            objects[index] = Handle(std::bit_cast<T*>(key), this);
            value.fetch_add(1);
            index++;
        }

        return objects;
    }

   private:
    void remove(uint8_t* ptr) override {
        std::bit_cast<T*>(ptr)->~T();
        ResourceRegistryBase::remove(ptr);
    }
};

template <typename T>
class ResourceIndex {
   public:
    bool add(std::string_view name, const Handle<T>& resource) {
        assert(name != "");

        return index.emplace(name, resource).second;
    }

    std::optional<Handle<T>> get(std::string_view name) const {
        auto it = index.find(name);
        if (it == index.end()) return std::nullopt;

        return it->second;
    }

    void remove(std::string_view name) { index.erase(name); }

   private:
    using Index = std::unordered_map<std::string, Handle<T>,
                                     TransparentStringHash, std::equal_to<>>;

    Index index;
};