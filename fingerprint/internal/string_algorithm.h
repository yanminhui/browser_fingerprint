#pragma once

#include <concepts>
#include <iterator>
#include <string>
#include <string_view>

namespace fingerprint {
namespace internal {

bool StrContainsIgnoreCase(std::string_view haystack,
                           std::string_view needle) noexcept;

bool EqualsIgnoreCase(std::string_view piece1,
                      std::string_view piece2) noexcept;

std::string_view StrStrip(std::string_view s) noexcept;

template <class T>
std::string ToString(T value) {
  return std::to_string(value);
}

std::string ToString(bool value);
std::string ToString(const std::string& value);
std::string ToString(std::string_view value);

bool StrToBool(std::string_view value);

template <std::same_as<bool> T>
T StrAs(const std::string& value) {
  return StrToBool(value);
}

template <std::unsigned_integral T>
T StrAs(const std::string& value) {
  constexpr auto size = sizeof(T);
  if constexpr (size == sizeof(unsigned long)) {
    return std::stoul(value);
  } else if constexpr (size == sizeof(unsigned long long)) {
    return std::stoull(value);
  } else {
    static_assert(size == sizeof(unsigned long) ||
                  size == sizeof(unsigned long long));
  }
}

template <std::floating_point T>
T StrAs(const std::string& value) {
  constexpr auto size = sizeof(T);
  if constexpr (size == sizeof(float)) {
    return std::stof(value);
  } else if constexpr (size == sizeof(double)) {
    return std::stod(value);
  } else {
    static_assert(size == sizeof(float) || size == sizeof(double));
  }
}

template <std::convertible_to<std::string> T>
T StrAs(const std::string& value) {
  return value;
}

template <class It>
std::string JoinRange(It first, It last, std::string_view separator) {
  std::string result;
  if (first != last) {
    using value_t = typename std::iterator_traits<It>::value_type;
    result = ToString(static_cast<const value_t&>(*first));
    while (++first != last) {
      result += separator;
      result += ToString(static_cast<const value_t&>(*first));
    }
  }
  return result;
}

std::string_view StrToken(std::string_view text, std::string_view delimiters,
                          const char** saveptr);

}  // namespace internal
}  // namespace fingerprint
