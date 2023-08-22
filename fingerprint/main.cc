#include <iostream>
#include <vector>

#include "fingerprint/fingerprint_context.h"
#include "fingerprint/services/path_service.h"
#include "fingerprint/services/use_service.h"
#include "fingerprint/settings.h"
#include "fingerprint/settings_keys.h"
#include "fingerprint/webgl_noise.h"
#include "fingerprint/webgl_renderer.h"

using namespace fingerprint;

Settings MakeSettings() {
  Settings st;

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
  FPcontext fp_context;

  // global path information
  auto& path_svc = UseService<PathService>(fp_context);
  std::cout << "HOME_DIR: " << path_svc.GetHomeDir() << std::endl;
  std::cout << "SETTINGS_PROFILE: " << path_svc.GetSettingsFile() << std::endl;

  // make settings
  auto st = MakeSettings();

  // save to disk
  std::function<std::string(const std::string&)> encode;
  SaveSettingsToFile(path_svc.GetSettingsFile(), st, encode);
  return 0;
}
