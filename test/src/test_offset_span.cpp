// Copyright (c) 2024 Pyarelal Knowles, MIT License

#include <gtest/gtest.h>
#include <nodecode/offset_span.hpp>

using namespace nodecode;

TEST(Span, ConstructorDefault) {
    offset_span<int> span;
    EXPECT_EQ(nullptr, span.data());
    EXPECT_EQ(0, span.size());
}

TEST(Span, ConstructorPtrSize) {
    int i[3];
    offset_span<int> span(i, std::size(i));
    EXPECT_EQ(i, span.data());
    EXPECT_EQ(3, span.size());
}

TEST(Span, ConstructorRange) {
    int i[3];
    offset_span<int> span(i);
    EXPECT_EQ(i, span.data());
    EXPECT_EQ(3, span.size());
}

TEST(Span, CopyAssignment) {
    int i[3];
    offset_span<int> span(i);
    offset_span span2 = span;
    EXPECT_EQ(i, span2.data());
}

TEST(Span, MoveAssignment) {
    int i[3];
    offset_span span = offset_span<int>(i);
    EXPECT_EQ(i, span.data());
}

TEST(Span, Iterate) {
    int i[3]{42, 42, 42};
    offset_span<int> span(i);
    for (int& v : span)
        EXPECT_EQ(42, v);
}

TEST(Span, TypeDeduction) {
    int i[3];
    offset_span span(i);
    EXPECT_EQ(i, span.data());
}

TEST(Span, FrontBack) {
    int i[]{1, 2, 3};
    offset_span span(i);
    EXPECT_EQ(span.front(), 1);
    EXPECT_EQ(span.back(), 3);
}

TEST(Span, Readme) {
    struct File {
        offset_span<int> values;
        int data[32];
    };

    File file;
    file.values = file.data;

    EXPECT_TRUE(file.values.data() == file.data);
    EXPECT_TRUE(file.values.size() == std::size(file.data));
};
