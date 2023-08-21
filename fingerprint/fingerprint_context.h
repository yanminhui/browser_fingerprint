#pragma once

#include "fingerprint/services/context.h"

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

  const Settings& GetSettings() const;
};

FPcontext* FPcontextPtr();

}  // namespace fingerprint
