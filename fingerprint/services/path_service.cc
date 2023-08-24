#include "fingerprint/services/path_service.h"

#include <cstdlib>
#include <string_view>
#include <system_error>

#include "fingerprint/internal/string_algorithm.h"

namespace {

constexpr std::string_view kSettingsProfile = ".fingerprint";

} // namespace

namespace fingerprint {

PathService::PathService(internal::Context& ctx)
    : internal::Context::Service(ctx) {
  // User's home directory.
  auto home_dir = []() -> fs::path { return std::getenv("HOME"); };
  RegisterProvider(PathKey::kDirHome, home_dir);

  // TODO:
  auto settings_profile = [this]() {
    return GetHomeDir() / kSettingsProfile;
  };
  RegisterProvider(PathKey::kFileSettings, settings_profile);
}

fs::path PathService::GetHomeDir() const { return Get(PathKey::kDirHome); }

fs::path PathService::GetSettingsFile() const {
  return Get(PathKey::kFileSettings);
}

fs::path PathService::Get(PathKey key) const {
  ProviderFunc path_func;
  {
    std::lock_guard<std::mutex> lock(mutex_);
    if (auto it = paths_.find(key); it != paths_.cend()) {
      path_func = it->second;
    }
  }
  if (!path_func) {
    auto ec = std::make_error_code(std::errc::invalid_argument);
    throw std::system_error{ec, "PathKey (" +
                                    internal::ToString(static_cast<int>(key)) +
                                    ") not exists"};
  }
  return path_func();
}

void PathService::RegisterProvider(PathKey key, ProviderFunc func) {
  std::lock_guard<std::mutex> lock(mutex_);
  paths_[key] = func;
}

}  // namespace fingerprint