#pragma once

#include <functional>
#include <string>

#include "fingerprint/services/context.h"
#include "fingerprint/services/path_service.h"
#include "fingerprint/settings.h"

namespace fingerprint {

class SettingsService : public internal::Context::Service {
 public:
  using ProviderFunc = std::function<std::string()>;
  using CryptFunc = std::function<std::string(const std::string&)>;

 public:
  explicit SettingsService(internal::Context& ctx);

  void RegisterProvider(ProviderFunc func);

  CryptFunc EncodeFunc() const;
  CryptFunc DecodeFunc() const;

  const Settings& GetSettings(bool* loaded = nullptr);

  std::string GetCipherData();
  std::string GetPlainData();

 private:
  bool LoadSettingsIfNeeded();

 private:
  PathService& path_service_;
  Settings settings_;
  ProviderFunc provider_func_;
};

}  // namespace fingerprint
