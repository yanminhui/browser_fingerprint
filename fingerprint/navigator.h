#pragma once

#include <cassert>

#include <string_view>

namespace fingerprint {

enum class PlatformId {
  kDefault, kMac, kWin
};

constexpr const char* NavigatorPlatform(PlatformId id) {
  if (id == PlatformId::kDefault) {
#if defined(__APPLE__)
    id = PlatformId::kMac;
#elif defined(_WIN32)
    id = PlatformId::kWin;
#else
#error Not Reached
#endif
  }
  switch (id)
  {
  case PlatformId::kMac:
    return "MacIntel";
  case PlatformId::kWin:
    return "Win32";
  default:
    assert(false && "Not Reached");
  }
  return "";
}

} // namespace fingerprint
