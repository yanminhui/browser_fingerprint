#pragma once

#include <type_traits>
#include <vector>

namespace fingerprint {
namespace internal {

template <class Container = std::vector<float>>
class ApplyFloatNoise {
 public:
  using ContainerType = Container;
  using ValueType = typename ContainerType::value_type;
  using SizeType = decltype(std::declval<ContainerType>().size());

  static_assert(std::is_floating_point<ValueType>::value);

 public:
  // [min, max]
  ApplyFloatNoise(ValueType min, ValueType max, ContainerType noises)
      : min_{min}, max_{max}, noises_{noises} {}

  ValueType operator()(ValueType value) {
    if (index_ < noises_.size()) {
      auto noise = noises_[index_];
      index_ = ++index_ % noises_.size();
      if (value + noise <= max_) {
        return value + noise;
      } else if (value - noise >= min_) {
        return value - noise;
      }
    }
    return value;
  }

  template <class It, class OutIt>
  void operator()(It first, It last, OutIt out) {
    while (first < last) {
      *out++ = (*this)(*first++);
    }
  }

 private:
  ValueType min_;
  ValueType max_;
  ContainerType noises_;
  SizeType index_ = 0;
};

}  // namespace internal
}  // namespace fingerprint
