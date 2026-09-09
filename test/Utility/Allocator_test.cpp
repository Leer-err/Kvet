#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "PoolAllocator.h"

struct TestObject {
    char* a;
};

TEST(AllocatorTest, allocationTest) {
    auto allocator = PoolAllocator(1, sizeof(TestObject), alignof(TestObject));

    auto ptr = allocator.allocate();

    ASSERT_NE(ptr, nullptr);
}

TEST(AllocatorTest, overAllocationTest) {
    auto allocator = PoolAllocator(1, sizeof(TestObject), alignof(TestObject));

    auto ptr = allocator.allocate();
    ptr = allocator.allocate();

    ASSERT_EQ(ptr, nullptr);
}