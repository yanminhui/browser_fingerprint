#include "fingerprint/fingerprint_context.h"

#include <cassert>

#include "fingerprint/services/settings_service.h"
#include "fingerprint/services/use_service.h"

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

void FPcontext::RegisterPathProvider(PathService::PathKey key,
                                     PathService::ProviderFunc func) {
  return internal::UseService<PathService>(*this).RegisterProvider(key, func);
}

void FPcontext::RegisterSettingsProvider(SettingsService::ProviderFunc func) {
  return internal::UseService<SettingsService>(*this).RegisterProvider(func);
}

SettingsService::CryptFunc FPcontext::GetSettingsEncodeFunc() {
  return internal::UseService<SettingsService>(*this).EncodeFunc();
}

std::string FPcontext::GetSettingsCipherData() {
  return internal::UseService<SettingsService>(*this).GetCipherData();
}

const Settings& FPcontext::GetSettings() {
  return internal::UseService<SettingsService>(*this).GetSettings();
}

FPcontext* FPcontextPtr() { return FPcontext::GetInstance(); }

}  // namespace fingerprint