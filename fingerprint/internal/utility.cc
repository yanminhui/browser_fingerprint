#include "fingerprint/internal/utility.h"

#include <cctype>

namespace {

constexpr auto kTrue = "true";
constexpr auto kFalse = "false";

}  // namespace

namespace fingerprint {
namespace internal {

bool StrContainsIgnoreCase(std::string_view haystack,
                           std::string_view needle) noexcept {
  using size_t = std::string_view::size_type;

  const size_t hsize = haystack.size();
  const size_t nsize = needle.size();
  if (hsize < nsize) {
    return false;
  }

  size_t i = 0;
  size_t j = 0;
  while (i < hsize && j < nsize) {
    if (std::tolower(haystack[i]) == std::tolower(needle[j])) {
      ++i;
      ++j;
    } else {
      i = i - j + 1;
      j = 0;
    }
  }
  return j >= nsize;
}

bool EqualsIgnoreCase(std::string_view piece1,
                      std::string_view piece2) noexcept {
  if (piece1.size() != piece2.size()) {
    return false;
  }
  for (std::string_view::size_type i = 0; i != piece1.size(); ++i) {
    if (std::tolower(piece1[i]) != std::tolower(piece2[i])) {
      return false;
    }
  }
  return true;
}

std::string ToString(bool value) { return value ? kTrue : kFalse; }

std::string ToString(const std::string& value) { return value; }

std::string ToString(std::string_view value) {
  return {value.data(), value.size()};
}

bool StrToBool(std::string_view value) {
  return EqualsIgnoreCase(value, kTrue);
}

std::string_view StrToken(std::string_view text, std::string_view delimiters,
                          const char** saveptr) {
  using size_t = std::string_view::size_type;

  size_t start_pos = 0;
  if (saveptr && *saveptr) {
    start_pos = *saveptr - text.data();
  }

  // no found
  if (start_pos >= text.size()) {
    if (saveptr) {
      *saveptr = nullptr;
    }
    return std::string_view{};
  }

  // start & continue
  size_t curr_pos = text.find_first_of(delimiters, start_pos);
  if (curr_pos != std::string_view::npos) {
    if (saveptr) {
      *saveptr = text.data() + curr_pos + 1;
    }
    return {text.data() + start_pos, curr_pos - start_pos};
  }

  // last
  if (saveptr) {
    *saveptr = text.data() + text.size();
  }
  return {text.data() + start_pos, text.size() - start_pos};
}

}  // namespace internal
}  // namespace fingerprint
