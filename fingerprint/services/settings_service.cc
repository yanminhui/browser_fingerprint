#include "fingerprint/services/settings_service.h"

#include "fingerprint/services/use_service.h"

namespace {

std::string Encode(const std::string& plaintext) {
  // TODO:
  return plaintext;
}

std::string Decode(const std::string& ciphertext) {
  // TODO:
  return ciphertext;
}

}  // namespace

namespace fingerprint {

SettingsService::SettingsService(internal::Context& ctx)
    : internal::Context::Service(ctx),
      path_service_(internal::UseService<PathService>(ctx)) {}

void SettingsService::RegisterProvider(ProviderFunc func) {
  provider_func_ = func;
}

const Settings& SettingsService::GetSettings() {
  LoadSettingsIfNeeded();
  return settings_;
}

std::string SettingsService::GetCipherData() { return Encode(GetPlainData()); }

std::string SettingsService::GetPlainData() {
  return SaveSettingsToText(GetSettings());
}

bool SettingsService::LoadSettingsIfNeeded() {
  if (loaded_) {
    return loaded_;
  }

  // load from provider
  if (provider_func_) {
    auto ciphertext = provider_func_();
    settings_ = LoadSettingsFromText(Decode(ciphertext));
    loaded_ = true;
    return true;
  }

  // load from disk
  auto file = path_service_.GetSettingsFile();
  if (fs::is_regular_file(file)) {
    settings_ = LoadSettingsFromFile(file, Decode);
    loaded_ = true;
    return true;
  }
  return false;
}

}  // namespace fingerprint
