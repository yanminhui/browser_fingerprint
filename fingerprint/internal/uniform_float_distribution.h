#pragma once

#include <chrono>
#include <random>
#include <type_traits>

namespace fingerprint {
namespace internal {

template <class T = double>
class UniformFloatDistribution {
 public:
  using ValueType = T;
  using Engine = std::default_random_engine;
  using Distribution = std::uniform_real_distribution<ValueType>;

  static_assert(std::is_floating_point<ValueType>::value);

 public:
  // [min, max)
  UniformFloatDistribution(ValueType min, ValueType max)
      : gen_{Seed()}, distribution_{min, max} {}

  ValueType operator()() { return distribution_(gen_); }

  template <class It>
  void operator()(It first, It last) {
    while (first < last) {
      *first++ = distribution_(gen_);
    }
  }

 private:
  static Engine::result_type Seed() {
    using SysClock = std::chrono::system_clock;
    constexpr auto max = (std::numeric_limits<Engine::result_type>::max)();
    auto s = SysClock::to_time_t(SysClock::now()) % max;
    return static_cast<Engine::result_type>(s);
  }

 private:
  Engine gen_;
  Distribution distribution_;
};

template <class It, class Value>
void FillRandomFloatArray(It first, It last, Value min, Value max) {
  UniformFloatDistribution<Value> ufd{min, max};
  ufd(first, last);
}

template <class It, class Size, class Value>
void FillRandomFloatArrayN(It first, Size n, Value min, Value max) {
  auto last = first;
  std::advance(first, n);
  FillRandomFloatArray(first, last, min, max);
}

} // namespace internal
}  // namespace fingerprint
