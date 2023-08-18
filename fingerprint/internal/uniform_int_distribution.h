#pragma once

#include <random>
#include <type_traits>

namespace fingerprint {
namespace internal {

template <class T = std::size_t>
class UniformIntDistribution {
 public:
  using ValueType = T;
  using Engine = std::default_random_engine;
  using Distribution = std::uniform_int_distribution<ValueType>;

  static_assert(std::is_integral<ValueType>::value);

 public:
  // [min, max]
  UniformIntDistribution(ValueType min, ValueType max)
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
    constexpr auto max = (std::numeric_limits<Engine::result_type>::max)();
    auto s = std::random_device{}() % max;
    return static_cast<Engine::result_type>(s);
  }

 private:
  Engine gen_;
  Distribution distribution_;
};

} // namespace internal
}  // namespace fingerprint
