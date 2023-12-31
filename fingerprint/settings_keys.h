#pragma once

namespace fingerprint {

// Navigator
constexpr const char gkNavigatorPlatform[] = "navigator.platform";  // string
constexpr const char gkNavigatorTimeZone[] = "navigator.timezone";  // string

// Proxy
constexpr const char gkProxyServer[] = "proxy.server";  // string
constexpr const char gkProxyRealm[] = "proxy.realm";    // string
constexpr const char gkProxyUser[] = "proxy.user";      // string
constexpr const char gkProxyPass[] = "proxy.pass";      // string

// WebRTC
constexpr const char gkWebRTCIPHandlingPolicy[] =
    "webrtc.ip_handling_policy";  // string

// WebGL
constexpr const char gkWebGLVendor[] = "webgl.vendor";             // string
constexpr const char gkWebGLRenderer[] = "webgl.renderer";         // string
constexpr const char gkWebGLImageNoises[] = "webgl.image_noises";  // float[N]

}  // namespace fingerprint
