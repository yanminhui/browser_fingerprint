#include <iostream>
#include <vector>

#include "fingerprint/fingerprint_context.h"
#include "fingerprint/navigator.h"
#include "fingerprint/services/path_service.h"
#include "fingerprint/services/use_service.h"
#include "fingerprint/settings.h"
#include "fingerprint/settings_keys.h"
#include "fingerprint/utility.h"
#include "fingerprint/webgl_noise.h"
#include "fingerprint/webgl_renderer.h"
#include "fingerprint/webrtc.h"

using namespace fingerprint;

Settings MakeSettings() {
  Settings st;

  // Navigator
  st.Put(gkNavigatorPlatform, NavigatorPlatform(PlatformId::kWin));
  st.Put(gkNavigatorTimeZone, "America/Los_Angeles");

  // Proxy
  st.Put(gkProxyServer, "socks5://bryan:abc123@localhost:7890");

  // WebRTC
  st.Put(gkWebRTCIPHandlingPolicy, WebRTCIPHandlingPolicy::kDisableNonProxiedUdp);

  // WebGL.vendor & renderer
  GLrendererPool pool;
  auto vendor = pool.NewVendor();
  st.Put(gkWebGLVendor, vendor);
  st.Put(gkWebGLRenderer, pool.NewRenderer(vendor));

  // WebGL.image_noisces
  std::vector<float> noises;
  GLnewNoiseFloatArray(noises);
  st.Put(gkWebGLImageNoises, noises.cbegin(), noises.cend());

  return st;
}

int main() {
  std::cout << "Random User Info\n================" << std::endl;
  std::cout << "Name:  "
            << NewRandomStr(gkUserNameLen, RandomChar::kUserNameRequired)
            << std::endl;
  std::cout << "Pass:  "
            << NewRandomStr(gkPasswordLen, RandomChar::kPasswordRequired)
            << std::endl;
  std::cout << std::endl;

  FPcontext fp_context;

  std::cout << "Global Path Info\n================" << std::endl;
  auto& path_svc = UseService<PathService>(fp_context);
  std::cout << "Home:    " << path_svc.GetHomeDir() << std::endl;
  std::cout << "Profile: " << path_svc.GetSettingsFile() << std::endl;
  std::cout << std::endl;

  // make settings
  auto st = MakeSettings();

  // save to disk
  SaveSettingsToFile(path_svc.GetSettingsFile(), st,
                     fp_context.GetSettingsEncodeFunc());
  return 0;
}
