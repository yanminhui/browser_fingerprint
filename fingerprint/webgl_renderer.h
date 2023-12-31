#pragma once

#include <string>
#include <unordered_map>
#include <unordered_set>

namespace fingerprint {

class GLrendererPool final {
 public:
  using SetContainer = std::unordered_set<std::string>;
  using RendererContainer = SetContainer;
  using VendorContainer = std::unordered_map<std::string, RendererContainer>;

 public:
  GLrendererPool();
  ~GLrendererPool();

  void Append(const std::string& vendor, const std::string& renderer);

  std::string NewVendor() const noexcept;
  std::string NewVendor(const std::string& keyword,
                        bool excluded) const noexcept;

  std::string NewRenderer(const std::string& vendor) const noexcept;

 private:
  VendorContainer vendor_2_renderers_;
};

}  // namespace fingerprint
