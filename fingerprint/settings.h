#pragma once

#include <string>
#include <unordered_map>

#include "fingerprint/internal/utility.h"

namespace fingerprint {

class Settings {
  static constexpr auto kSeparator = ";";

 public:
  void Put(const std::string& key, const std::string& value);
  void Put(const std::string& key, std::size_t value);
  void Put(const std::string& key, double value);
  void Put(const std::string& key, bool value);

  template <class It>
  void Put(const std::string& key, It first, It last) {
    Put(key, internal::JoinRange(first, last, kSeparator));
  }

  std::string GetString(const std::string& key) const;
  std::size_t GetUInt(const std::string& key) const;
  double GetDouble(const std::string& key) const;
  bool GetBool(const std::string& key) const;

  template <class Container>
  void Get(const std::string& key, Container& value) const;

 private:
  void PutInternal(const std::string& key, const std::string& value);
  std::string GetInternal(const std::string& key) const;

 private:
  std::unordered_map<std::string, std::string> storage_;
};

template <class Container>
void Settings::Get(const std::string& key, Container& value) const {
  using value_t = typename Container::value_type;

  auto result = GetString(key);
  const char* saveptr = nullptr;
  for (auto item = internal::StrToken(result, kSeparator, &saveptr); saveptr;
       item = internal::StrToken(result, kSeparator, &saveptr)) {
    value.push_back(
        internal::StrAs<value_t>(std::string{item.data(), item.size()}));
  }
}

void LoadSettingsFromJson(const std::string& name, Settings& settings);
void LoadSettingsFromLevelDB(const std::string& name, Settings& settings);

}  // namespace fingerprint
