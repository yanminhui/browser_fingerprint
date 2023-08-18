#include <cctype>

#include "fingerprint/internal/utility.h"

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

}  // namespace internal
}  // namespace fingerprint
