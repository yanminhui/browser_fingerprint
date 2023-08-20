#include "fingerprint/settings.h"

#include <string_view>
#include <system_error>

namespace fingerprint {

void Settings::Put(const std::string& key, const std::string& value) {
  PutInternal(key, value);
}

void Settings::Put(const std::string& key, std::size_t value) {
  PutInternal(key, internal::ToString(value));
}

void Settings::Put(const std::string& key, double value) {
  PutInternal(key, internal::ToString(value));
}

void Settings::Put(const std::string& key, bool value) {
  PutInternal(key, internal::ToString(value));
}

std::string Settings::GetString(const std::string& key) const {
  return GetInternal(key);
}

std::size_t Settings::GetUInt(const std::string& key) const {
  return internal::StrAs<std::size_t>(GetInternal(key));
}

double Settings::GetDouble(const std::string& key) const {
  return internal::StrAs<double>(GetInternal(key));
}

bool Settings::GetBool(const std::string& key) const {
  return internal::StrToBool(GetInternal(key));
}

void Settings::PutInternal(const std::string& key, const std::string& value) {
  storage_[key] = value;
}

std::string Settings::GetInternal(const std::string& key) const {
  if (auto it = storage_.find(key); it != storage_.cend()) {
    return it->second;
  }
  auto ec = std::make_error_code(std::errc::invalid_argument);
  throw std::system_error{ec, "\"" + key + "\" not found in settings"};
}

void LoadSettingsFromJson(const std::string& name, Settings& settings) {
  // TODO:
}

void LoadSettingsFromLevelDB(const std::string& name, Settings& settings) {
  // TODO:
}

}  // namespace fingerprint
