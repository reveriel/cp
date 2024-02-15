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
