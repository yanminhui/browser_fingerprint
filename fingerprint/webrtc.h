#pragma once

#include <string_view>

namespace fingerprint {

enum WebRTCIPHandlingPolicy : std::size_t {
  kDefault,
  kDefaultPublicAndPrivateInterfaces,
  kDefaultPublicInterfaceOnly,
  kDisableNonProxiedUdp,
};

std::string_view GetWebRTCIPHandlingPolicySwitch(WebRTCIPHandlingPolicy policy);

}  // namespace fingerprint
