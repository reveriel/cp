
// simple timestamp, wrap the <chrono> library

#pragma once

#include <chrono>
#include <string>

namespace cp {

class TimeDifference;

class Timestamp {
public:
  Timestamp() : m_microSecondsSinceEpoch(0) {}
  explicit Timestamp(int64_t microSecondsSinceEpoch)
      : m_microSecondsSinceEpoch(microSecondsSinceEpoch) {}

  void swap(Timestamp &that) {
    std::swap(m_microSecondsSinceEpoch, that.m_microSecondsSinceEpoch);
  }

  std::string toString() const;
  std::string toFormattedString(bool showMicroseconds = true) const;

  bool valid() const { return m_microSecondsSinceEpoch > 0; }

  // for internal usage.
  int64_t microSecondsSinceEpoch() const { return m_microSecondsSinceEpoch; }
  time_t secondsSinceEpoch() const {
    return static_cast<time_t>(m_microSecondsSinceEpoch /
                               kMicroSecondsPerSecond);
  }

  // operators
  bool operator<(const Timestamp &rhs) const {
    return m_microSecondsSinceEpoch < rhs.m_microSecondsSinceEpoch;
  }

  bool operator==(const Timestamp &rhs) const {
    return m_microSecondsSinceEpoch == rhs.m_microSecondsSinceEpoch;
  }
  bool operator!=(const Timestamp &rhs) const {
    return m_microSecondsSinceEpoch != rhs.m_microSecondsSinceEpoch;
  }
  bool operator<=(const Timestamp &rhs) const {
    return m_microSecondsSinceEpoch <= rhs.m_microSecondsSinceEpoch;
  }
  bool operator>=(const Timestamp &rhs) const {
    return m_microSecondsSinceEpoch >= rhs.m_microSecondsSinceEpoch;
  }
  bool operator>(const Timestamp &rhs) const {
    return m_microSecondsSinceEpoch > rhs.m_microSecondsSinceEpoch;
  }

  TimeDifference operator-(const Timestamp &rhs);

  static Timestamp now();
  static Timestamp invalid() { return Timestamp(); }

  static Timestamp fromUnixTime(time_t t) { return fromUnixTime(t, 0); }
  static Timestamp fromUnixTime(time_t t, int microseconds) {
    return Timestamp(static_cast<int64_t>(t) * kMicroSecondsPerSecond +
                     microseconds);
  }

  static const int kMicroSecondsPerSecond = 1000 * 1000;

private:
  int64_t m_microSecondsSinceEpoch;
};

class TimeDifference {
public:
  TimeDifference() : m_microSeconds(0) {}
  TimeDifference(int64_t microSeconds) : m_microSeconds(microSeconds) {}
  TimeDifference(double seconds)
      : m_microSeconds(
            static_cast<int64_t>(seconds * Timestamp::kMicroSecondsPerSecond)) {
  }
  TimeDifference(const Timestamp &high, const Timestamp &low)
      : m_microSeconds(high.microSecondsSinceEpoch() -
                       low.microSecondsSinceEpoch()) {}

  double seconds() const {
    return static_cast<double>(m_microSeconds) /
           Timestamp::kMicroSecondsPerSecond;
  }
  int64_t microSeconds() const { return m_microSeconds; }
  // operators
  bool operator<(const TimeDifference &rhs) const {
    return m_microSeconds < rhs.m_microSeconds;
  }

  bool operator==(const TimeDifference &rhs) const {
    return m_microSeconds == rhs.m_microSeconds;
  }
  bool operator!=(const TimeDifference &rhs) const {
    return m_microSeconds != rhs.m_microSeconds;
  }
  bool operator<=(const TimeDifference &rhs) const {
    return m_microSeconds <= rhs.m_microSeconds;
  }
  bool operator>=(const TimeDifference &rhs) const {
    return m_microSeconds >= rhs.m_microSeconds;
  }
  bool operator>(const TimeDifference &rhs) const {
    return m_microSeconds > rhs.m_microSeconds;
  }

  TimeDifference operator+(const TimeDifference &rhs) {
    return TimeDifference(m_microSeconds + rhs.m_microSeconds);
  }

  TimeDifference operator-(const TimeDifference &rhs) {
    return TimeDifference(m_microSeconds - rhs.m_microSeconds);
  }

  TimeDifference &operator+=(const TimeDifference &rhs) {
    m_microSeconds += rhs.m_microSeconds;
    return *this;
  }

  TimeDifference &operator-=(const TimeDifference &rhs) {
    m_microSeconds -= rhs.m_microSeconds;
    return *this;
  }

  // convert to double
  double toMilliseconds() const {
    return static_cast<double>(m_microSeconds) / 1000;
  }

private:
  int64_t m_microSeconds;
};

} // namespace cp
