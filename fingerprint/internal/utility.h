#pragma once

#include <string_view>

namespace fingerprint {
namespace internal {

bool StrContainsIgnoreCase(std::string_view haystack,
                           std::string_view needle) noexcept;

}  // namespace internal
}  // namespace fingerprint
