#include <iostream>
#include <vector>

#include "fingerprint/settings.h"
#include "fingerprint/webgl_noise.h"
#include "fingerprint/webgl_renderer.h"

using namespace fingerprint;

int main() {
  Settings st;

  GLrendererPool pool;
  auto vendor = pool.NewVendor();
  st.Put("webgl.vendor", vendor);
  st.Put("webgl.renderer", pool.NewRenderer(vendor));

  std::vector<float> noises;
  GLnewNoiseFloatArray(noises);
  noises.resize(5);
  st.Put("webgl.noises", noises.cbegin(), noises.cend());

  auto text = SaveSettingsToText(st);
  std::cout << "[settings]:\n" << text << std::endl;

  auto st2 = LoadSettingsFromText(text);
  std::cout << st2.GetString("webgl.vendor") << std::endl;
  std::cout << st2.GetString("webgl.renderer") << std::endl;
  std::cout << st2.GetString("webgl.noises") << std::endl;

  std::vector<float> result;
  st2.Get("webgl.noises", result);
  for (auto i = 0; i != result.size(); ++i) {
    std::cout << i + 1 << ". " << std::fixed << result[i] << std::endl;
  }
  return 0;
}
