#pragma once

#include "fingerprint/services/context.h"
#include "fingerprint/services/path_service.h"

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

  const Settings& GetSettings() const;
};

FPcontext* FPcontextPtr();

}  // namespace fingerprint
