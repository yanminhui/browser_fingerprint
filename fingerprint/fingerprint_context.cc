#include "fingerprint/fingerprint_context.h"

#include <cassert>

#include "fingerprint/services/settings_service.h"

namespace {

fingerprint::FPcontext* s_fp_context = nullptr;

}  // namespace

namespace fingerprint {

FPcontext* FPcontext::GetInstance() {
  assert(s_fp_context &&
         "FPcontext must keep alive in application's lifecycle.");
  return s_fp_context;
}

FPcontext::FPcontext() { s_fp_context = this; }

FPcontext::~FPcontext() { s_fp_context = nullptr; }

const Settings& FPcontext::GetSettings() const {
  return internal::UseService<SettingsService>(const_cast<FPcontext&>(*this))
      .GetSettings();
}

FPcontext* FPcontextPtr() { return FPcontext::GetInstance(); }

}  // namespace fingerprint