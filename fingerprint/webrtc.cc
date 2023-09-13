#include "fingerprint/webrtc.h"

#include <cassert>

namespace fingerprint {

std::string_view GetWebRTCIPHandlingPolicySwitch(
    WebRTCIPHandlingPolicy policy) {
  std::string_view result("default");
  switch (policy) {
    case WebRTCIPHandlingPolicy::kDefault:
      result = "default";
      break;
    case WebRTCIPHandlingPolicy::kDefaultPublicAndPrivateInterfaces:
      result = "default_public_and_private_interfaces";
      break;
    case WebRTCIPHandlingPolicy::kDefaultPublicInterfaceOnly:
      result = "default_public_interface_only";
      break;
    case WebRTCIPHandlingPolicy::kDisableNonProxiedUdp:
      result = "disable_non_proxied_udp";
      break;
    default:
      assert(false && "Not Reached");
      break;
  }
  return result;
}

}  // namespace fingerprint
