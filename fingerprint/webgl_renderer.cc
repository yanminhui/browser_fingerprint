#include "fingerprint/webgl_renderer.h"

#include <iterator>

#include "fingerprint/internal/uniform_int_distribution.h"
#include "fingerprint/internal/utility.h"

namespace fingerprint {

GLrendererPool::GLrendererPool() {
  Append("Apple Inc.", "Apple GPU");
  Append("Google Inc. (Apple)", "ANGLE (Apple, Apple M2, OpenGL 4.1)");
  Append("Google Inc. (Apple)",
         "ANGLE (Apple, ANGLE Metal Renderer: Apple M2, Unspecified Version)");
  // TODO: add more...
}

GLrendererPool::~GLrendererPool() {}

void GLrendererPool::Append(const std::string& vendor,
                            const std::string& renderer) {
  vendor_2_renderers_[vendor].insert(renderer);
}

std::string GLrendererPool::NewVendor() const noexcept {
  const std::string empty_keyword;
  return NewVendor(empty_keyword);
}

std::string GLrendererPool::NewVendor(
    const std::string& keyword) const noexcept {
  std::vector<std::string> vendors;
  for (const auto& [k, v] : vendor_2_renderers_) {
    if (keyword.empty() || internal::StrContainsIgnoreCase(k, keyword)) {
      vendors.push_back(k);
    }
  }
  if (vendors.size() > 0) {
    internal::UniformIntDistribution<std::size_t> uid{0, vendors.size() - 1};
    return vendors[uid()];
  }
  return "";
}

std::string GLrendererPool::NewRenderer(
    const std::string& vendor) const noexcept {
  if (auto it = vendor_2_renderers_.find(vendor);
      it != vendor_2_renderers_.cend() && !it->second.empty()) {
    internal::UniformIntDistribution<std::size_t> uid{0, it->second.size() - 1};
    auto first = it->second.cbegin();
    std::advance(first, uid());
    return *first;
  }
  return "";
}

}  // namespace fingerprint
