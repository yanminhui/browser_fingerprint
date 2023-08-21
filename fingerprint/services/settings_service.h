#pragma once

#include "fingerprint/services/context.h"
#include "fingerprint/services/path_service.h"
#include "fingerprint/settings.h"

namespace fingerprint {

class SettingsService : public internal::Context::Service {
 public:
  explicit SettingsService(internal::Context& ctx);

  const Settings& GetSettings() const;

 private:
  PathService& path_service_;
  Settings settings_;
};

}  // namespace fingerprint
