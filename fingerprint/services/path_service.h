#pragma once

#include <filesystem>
#include <functional>
#include <mutex>
#include <unordered_map>

#include "fingerprint/services/context.h"

namespace fingerprint {

namespace fs = std::filesystem;

// references:
// - chromium: enum BasePathKey
// - chromium: RegisterPathProvider() -> GetPathData() -> PathService
class PathService : public internal::Context::Service {
 public:
  enum PathKey {
    kDirHome,
    kFileSettings,
  };

  using ProviderFunc = std::function<fs::path()>;

 public:
  explicit PathService(internal::Context& ctx);

  // User's root home directory.
  fs::path GetHomeDir() const;

  // fingerprint's settings path.
  fs::path GetSettingsFile() const;

 public:
  fs::path Get(PathKey key) const;

  void RegisterProvider(PathKey key, ProviderFunc func);

 private:
  mutable std::mutex mutex_;
  std::unordered_map<PathKey, ProviderFunc> paths_;
};

}  // namespace fingerprint
