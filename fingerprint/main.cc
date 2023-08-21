#include <iostream>
#include <vector>

#include "fingerprint/fingerprint_context.h"
#include "fingerprint/services/path_service.h"
#include "fingerprint/settings.h"
#include "fingerprint/settings_keys.h"
#include "fingerprint/webgl_noise.h"
#include "fingerprint/webgl_renderer.h"

using namespace fingerprint;

int main() {
  FPcontext fp_context;
  auto& path_svc = UseService<PathService>(fp_context);
  std::cout << "HOME_DIR: " << path_svc.GetHomeDir() << std::endl;
  std::cout << "SETTINGS_PROFILE: " << path_svc.GetSettingsFile() << std::endl;
  Settings st;
  GLrendererPool pool;
  auto vendor = pool.NewVendor();
  st.Put(gkWebGLVendor, vendor);
  st.Put(gkWebGLRenderer, pool.NewRenderer(vendor));

  std::vector<float> noises;
  GLnewNoiseFloatArray(noises);
  st.Put(gkWebGLImageNoises, noises.cbegin(), noises.cend());

  auto encode = [](const std::string& plaintext){
    return plaintext;
  };
  SaveSettingsToFile(path_svc.GetSettingsFile(), st, encode);
  return 0;
}
