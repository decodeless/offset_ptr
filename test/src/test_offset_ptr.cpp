// Copyright (c) 2024 Pyarelal Knowles, MIT License

#include <cstring>
#include <gtest/gtest.h>
#include <decodeless/offset_ptr.hpp>

using namespace decodeless;

static_assert(std::is_trivially_destructible_v<offset_ptr<int>>);

TEST(Ptr, ConstructorDefault) {
    offset_ptr<int> ptr;
    EXPECT_EQ(nullptr, ptr.get());
}

TEST(Ptr, ConstructorPointer) {
    int i;
    offset_ptr<int> ptr(&i);
    EXPECT_EQ(&i, ptr.get());
}

TEST(Ptr, ConstructorMove) {
    int i;
    auto ptr = offset_ptr<int>(&i);
    EXPECT_EQ(&i, ptr.get());
}

TEST(Ptr, CopyAssignment) {
    int i;
    offset_ptr<int> ptr(&i);
    EXPECT_EQ(&i, ptr.get());
    offset_ptr<int> ptr2 = ptr;
    EXPECT_EQ(&i, ptr2.get());
}

TEST(Ptr, SetValid) {
    int i, j;
    offset_ptr<int> ptr(&i);
    EXPECT_EQ(&i, ptr.get());
    ptr.set(&j);
    EXPECT_EQ(&j, ptr.get());
}

TEST(Ptr, SetNull) {
    int i;
    offset_ptr<int> ptr(&i);
    EXPECT_EQ(&i, ptr.get());
    ptr.set(nullptr);
    EXPECT_EQ(nullptr, ptr.get());
}

TEST(Ptr, SetSelf) {
    struct S {
        offset_ptr<S> s;
    };
    S s;
    s.s = &s;
    EXPECT_EQ(&s, s.s.get());
    EXPECT_EQ(&s, &*s.s->s);
    EXPECT_EQ(&s, &*s.s->s->s);
}

TEST(Ptr, Dereference) {
    int i = 42;
    offset_ptr<int> ptr(&i);
    EXPECT_EQ(42, *ptr);
}

TEST(Ptr, Arrow) {
    struct S {
        int i;
    };
    S s{42};
    offset_ptr<S> ptr(&s);
    EXPECT_EQ(42, ptr->i);
}

TEST(Ptr, Array) {
    int i[] = {0, 1, 2};
    offset_ptr<int> ptr(i);
    EXPECT_EQ(0, ptr[0]);
    EXPECT_EQ(1, ptr[1]);
    EXPECT_EQ(2, ptr[2]);
}

TEST(Ptr, Equality) {
    int i = 42;
    offset_ptr<int> ptr(&i);
    EXPECT_TRUE(ptr == &i);
    EXPECT_TRUE(ptr == offset_ptr(&i));
    EXPECT_FALSE(ptr != &i);
    EXPECT_FALSE(ptr != offset_ptr(&i));
    int j = 0;
    EXPECT_TRUE(ptr != &j);
    EXPECT_TRUE(ptr != offset_ptr(&j));
}

TEST(Ptr, Readme) {
    struct File {
        offset_ptr<int> ptr;
        int data;
    };

    File file;
    file.data = 42;
    file.ptr =
        &file.data; // intialize the offset pointer from a regular pointer

    // Duplicate the memory. file2's ptr implicitly points to file2's data
    File file2;
    std::memcpy(reinterpret_cast<void*>(&file2), &file, sizeof(file));
    EXPECT_TRUE(file2.ptr == &file2.data);
    EXPECT_TRUE(*file2.ptr == 42);

    // Pointer data is mutable
    *file2.ptr = 21;
    EXPECT_TRUE(file2.data == 21);

    // Prove we weren't accidentally modifying the original file
    EXPECT_TRUE(file.data == 42);
}
