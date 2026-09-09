#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utility>

#include "ResourceRegistry.h"

struct TestObject {
    char* a;
};

TEST(ResourceRegistryTest, allocationTest) {
    auto allocator = PoolAllocator(1, sizeof(TestObject), alignof(TestObject));

    auto registry = ResourceRegistry<TestObject>(allocator);
    auto handle = registry.create((char*)1);

    ASSERT_TRUE(handle.isValid());
    ASSERT_EQ(handle->a, (char*)1);
}

TEST(ResourceRegistryTest, handleCopyTest) {
    auto allocator = PoolAllocator(1, sizeof(TestObject), alignof(TestObject));

    auto registry = ResourceRegistry<TestObject>(allocator);
    auto handle = registry.create((char*)1);

    {
        auto other_handle = handle;

        ASSERT_TRUE(handle.isValid());
        ASSERT_TRUE(other_handle.isValid());
        ASSERT_EQ(&handle.get(), &other_handle.get());
        ASSERT_EQ(other_handle->a, (char*)1);
    }

    ASSERT_EQ(handle->a, (char*)1);
}

TEST(ResourceRegistryTest, handleMoveTest) {
    auto allocator = PoolAllocator(1, sizeof(TestObject), alignof(TestObject));

    auto registry = ResourceRegistry<TestObject>(allocator);
    auto handle = registry.create((char*)1);

    {
        auto other_handle = std::move(handle);

        ASSERT_TRUE(other_handle.isValid());
        ASSERT_EQ(other_handle->a, (char*)1);
    }

    ASSERT_FALSE(handle.isValid());
}