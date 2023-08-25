#pragma once

#include "fingerprint/services/context.h"
#include "fingerprint/services/path_service.h"
#include "fingerprint/services/settings_service.h"

namespace fingerprint {

class Settings;

// references:
// - chromium: ChromeMain()
class FPcontext : public internal::Context {
 public:
  static FPcontext* GetInstance();

 public:
  FPcontext();
  ~FPcontext() override;

  void RegisterPathProvider(PathService::PathKey key,
                            PathService::ProviderFunc func);

  // called by render: get settings ciphertext from browser.
  void RegisterSettingsProvider(SettingsService::ProviderFunc func);

  SettingsService::CryptFunc GetSettingsEncodeFunc();

  // called by browser: send ciphertext to render.
  std::string GetSettingsCipherData();

  const Settings& GetSettings(bool* loaded = nullptr);
};

FPcontext* FPcontextPtr();

}  // namespace fingerprint
