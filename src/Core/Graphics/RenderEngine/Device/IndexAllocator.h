#pragma once

#include <optional>
#include <vector>

template <typename T>
class IndexAllocator {
   public:
    explicit IndexAllocator(T index_count) : count(index_count) {
        free_indices.reserve(count);

        for (auto i = count; i > 0; i--) free_indices.push_back(i);
    }

    std::optional<T> allocate() {
        if (free_indices.empty()) return std::nullopt;

        auto index = free_indices.back();
        free_indices.pop_back();

        return index;
    }

    void free(T index) { free_indices.push_back(index); }

   private:
    T count;
    std::vector<T> free_indices;
};
