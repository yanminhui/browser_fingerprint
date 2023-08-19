#include <iostream>

#include "fingerprint/webgl_renderer.h"

int main() {
  fingerprint::GLrendererPool pool;
  for (auto i = 0; i != 10; ++i) {
    auto vendor = pool.NewVendor();
    std::cout << i << ". " << vendor << "-" << pool.NewRenderer(vendor)
              << std::endl;
  }
  return 0;
}
