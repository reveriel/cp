
#include "log_stream.h"

namespace cp {

namespace detail {

const char digits[] = "9876543210123456789";
const char* zero = digits + 9;
static_assert(sizeof(digits) == 20, "wrong digits");

const char digitsHex[] = "0123456789ABCDEF";
static_assert(sizeof(digitsHex) == 17, "wrong digitsHex");

// convrt interger to string
template <typename T>
size_t convert(char buf[], T value) {
    T i = value;
    char *p = buf;

    do {
        int lsd = static_cast<int>(i % 10);
        i /= 10;
        *p++ = zero[lsd];
    } while (i != 0);

    if (value < 0) {
        *p++ = '-';
    }
    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

size_t convertHex(char buf[], uintptr_t value) {
    uintptr_t i = value;
    char *p = buf;

    do {
        int lsd = i % 16;
        i /= 16;
        *p++ = digitsHex[lsd];
    } while (i != 0);

    *p = '\0';
    std::reverse(buf, p);

    return p - buf;
}

} // namespace detail

void LogStream::staticCheck() {
    static_assert(kMaxNumericSize - 1 > std::numeric_limits<int>::digits10,
                  "kMaxNumericSize is too small");
    static_assert(kMaxNumericSize - 1 > std::numeric_limits<long>::digits10,
                  "kMaxNumericSize is too small");
    static_assert(kMaxNumericSize - 1 > std::numeric_limits<long long>::digits10,
                  "kMaxNumericSize is too small");

    static_assert(kMaxNumericSize - 1 > std::numeric_limits<unsigned int>::digits10,
                  "kMaxNumericSize is too small");
    static_assert(kMaxNumericSize - 1 > std::numeric_limits<unsigned long>::digits10,
                  "kMaxNumericSize is too small");
    static_assert(kMaxNumericSize - 1 > std::numeric_limits<unsigned long long>::digits10,
                  "kMaxNumericSize is too small");
}

template <typename T>
void LogStream::formatInteger(T v) {
    if (buffer_.avail() >= kMaxNumericSize) {
        size_t len = detail::convert(buffer_.current(), v);
        buffer_.add(len);
    }
}

LogStream& LogStream::operator<<(bool v) {
    buffer_.append(v ? "1" : "0", 1);
    return *this;
}

LogStream& LogStream::operator<<(short v) {
    *this << static_cast<int>(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned short v) {
    *this << static_cast<unsigned int>(v);
    return *this;
}

LogStream& LogStream::operator<<(int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned int v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(unsigned long long v) {
    formatInteger(v);
    return *this;
}

LogStream& LogStream::operator<<(float v) {
    *this << static_cast<double>(v);
    return *this;
}

LogStream& LogStream::operator<<(double v) {
    if (buffer_.avail() >= kMaxNumericSize) {
        size_t len = snprintf(buffer_.current(), kMaxNumericSize, "%.12g", v);
        buffer_.add(len);
    }
    return *this;
}


} // namespace cp
