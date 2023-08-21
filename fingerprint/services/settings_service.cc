#include "fingerprint/services/settings_service.h"

#include "fingerprint/services/use_service.h"

namespace fingerprint {

SettingsService::SettingsService(internal::Context& ctx)
    : internal::Context::Service(ctx),
      path_service_(internal::UseService<PathService>(ctx)) {
  auto file = path_service_.GetSettingsFile();
  if (fs::is_regular_file(file)) {
    // TODO:
    auto decode = [](const std::string& encrypttext) { return encrypttext; };
    settings_ = LoadSettingsFromFile(file, decode);
  }
}

const Settings& SettingsService::GetSettings() const {
  return settings_;
}

}  // namespace fingerprint
