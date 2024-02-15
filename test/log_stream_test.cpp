#include "log_stream.h"
#include <gtest/gtest.h>

using namespace cp;

TEST(LogStreamTest, TestBoolean) {
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    EXPECT_EQ(buf.toString(), "");
    os << true;
    EXPECT_EQ(buf.toString(), "1");
    os << '\n';
    EXPECT_EQ(buf.toString(), "1\n");
    os << false;
    EXPECT_EQ(buf.toString(), "1\n0");
}

TEST(LogStreamTest, TestInt) {
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    EXPECT_EQ(buf.toString(), "");
    os << 1 << '\n';
    EXPECT_EQ(buf.toString(), "1\n");
    os << 0;
    EXPECT_EQ(buf.toString(), "1\n0");
    os << -1;
    EXPECT_EQ(buf.toString(), "1\n0-1");
    os << '\n' << 1000;
    EXPECT_EQ(buf.toString(), "1\n0-1\n1000");
}

TEST(LogStreamTest, TestIntLimit) {
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    os << -2147483647;
    EXPECT_EQ(buf.toString(), "-2147483647");
    os << -2147483648;
    EXPECT_EQ(buf.toString(), "-2147483647-2147483648");
    os << ' ' << 0x7FFFFFFF;
    EXPECT_EQ(buf.toString(), "-2147483647-2147483648 2147483647");

    os.reset();
    os << std::numeric_limits<int16_t>::min();
    EXPECT_EQ(buf.toString(), "-32768");

    os << ' ' << std::numeric_limits<int16_t>::max();
    EXPECT_EQ(buf.toString(), "-32768 32767");

    os.reset();
    os << std::numeric_limits<int32_t>::min();
    EXPECT_EQ(buf.toString(), "-2147483648");

    os << ' ' << std::numeric_limits<int32_t>::max();
    EXPECT_EQ(buf.toString(), "-2147483648 2147483647");

    os.reset();
    os << std::numeric_limits<int64_t>::min();
    EXPECT_EQ(buf.toString(), "-9223372036854775808");

    os << ' ' << std::numeric_limits<int64_t>::max();
    EXPECT_EQ(buf.toString(), "-9223372036854775808 9223372036854775807");

    os.reset();
    os << std::numeric_limits<uint64_t>::max();
    EXPECT_EQ(buf.toString(), "18446744073709551615");
}

TEST(LogStreamTest, TestDouble) {
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    os << 0.0;
    EXPECT_EQ(buf.toString(), "0");
    os << 1.0;
    EXPECT_EQ(buf.toString(), "01");
    os << 0.1;
    EXPECT_EQ(buf.toString(), "010.1");
    os << 0.05;
    EXPECT_EQ(buf.toString(), "010.10.05");
    os << 1.5;
    EXPECT_EQ(buf.toString(), "010.10.051.5");

    os.reset();
    os << -1.0;
    EXPECT_EQ(buf.toString(), "-1");

    os.reset();
    os << -0.1;
    EXPECT_EQ(buf.toString(), "-0.1");

    os.reset();
    os << -0.05;
    EXPECT_EQ(buf.toString(), "-0.05");

    os.reset();
    os << -0.15;
    EXPECT_EQ(buf.toString(), "-0.15");
}

TEST(LogStreamTest, TestString) {
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    os << "hello";
    EXPECT_EQ(buf.toString(), "hello");
}

TEST(LogStreamTest, TestBuffer) {
    LogStream os;
    const LogStream::Buffer &buf = os.buffer();
    os << "hello" << "world";
    EXPECT_EQ(buf.toString(), "helloworld");
}
