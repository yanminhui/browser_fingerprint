#include "fingerprint/services/settings_service.h"

#include "fingerprint/crypt/base64.h"
#include "fingerprint/services/use_service.h"
#include "fingerprint/utility.h"

namespace {

constexpr std::string_view kPass = "WhrnQK-xfPqGr-yDHHGY";

std::string Encode(const std::string& plaintext) {
  return base64_encode(fingerprint::TwofishEncrypt(plaintext, kPass));
}

std::string Decode(const std::string& ciphertext) {
  return fingerprint::TwofishDecrypt(base64_decode(ciphertext), kPass);
}

}  // namespace

namespace fingerprint {

SettingsService::SettingsService(internal::Context& ctx)
    : internal::Context::Service(ctx),
      path_service_(internal::UseService<PathService>(ctx)) {}

void SettingsService::RegisterProvider(ProviderFunc func) {
  provider_func_ = func;
}

SettingsService::CryptFunc SettingsService::EncodeFunc() const {
  return Encode;
}

SettingsService::CryptFunc SettingsService::DecodeFunc() const {
  return Decode;
}

const Settings& SettingsService::GetSettings(bool* loaded) {
  if (loaded) {
    *loaded = LoadSettingsIfNeeded();
  } else {
    LoadSettingsIfNeeded();
  }
  return settings_;
}

std::string SettingsService::GetCipherData() {
  return Encode(GetPlainData());
}

std::string SettingsService::GetPlainData() {
  return SaveSettingsToText(GetSettings());
}

bool SettingsService::LoadSettingsIfNeeded() {
  static bool loaded = [this]() {
    // load from provider
    if (provider_func_) {
      auto ciphertext = provider_func_();
      settings_ = LoadSettingsFromText(Decode(ciphertext));
      return true;
    }
    // load from disk
    auto file = path_service_.GetSettingsFile();
    if (fs::is_regular_file(file)) {
      settings_ = LoadSettingsFromFile(file, Decode);
      return true;
    }
    return false;
  }(); // call once
  return loaded;
}

}  // namespace fingerprint
