// Copyright (c) 2024 Pyarelal Knowles, MIT License

#include <decodeless/offset_span.hpp>
#include <gtest/gtest.h>
#include <vector>

using namespace decodeless;

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

TEST(Span, ConstructorSpan) {
    int i[3];
    std::span<int> stdspan(i);
    offset_span<int> span(stdspan);
    EXPECT_EQ(i, span.data());
    EXPECT_EQ(3, span.size());
    span = stdspan;
    EXPECT_EQ(i, span.data());
    EXPECT_EQ(3, span.size());
}

TEST(Span, ConstructorSpanConst) {
    int i[3];
    std::span<int> stdspan(i);
    offset_span<const int> span(stdspan);
    EXPECT_EQ(i, span.data());
    EXPECT_EQ(3, span.size());
    span = stdspan;
    EXPECT_EQ(i, span.data());
    EXPECT_EQ(3, span.size());
}

TEST(Span, ConstructorRange) {
    int i[3];
    offset_span<int> span(i);
    EXPECT_EQ(i, span.data());
    EXPECT_EQ(3, span.size());
}

TEST(Span, ConstructorVector) {
    std::vector<int> i(3);
    offset_span<int> span(i);
    EXPECT_EQ(i.data(), span.data());
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
