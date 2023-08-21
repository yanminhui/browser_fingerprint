#include "fingerprint/settings.h"

#include <cassert>
#include <fstream>
#include <string_view>
#include <system_error>

namespace {

constexpr std::size_t kSettingsCapacity = 4096u;
constexpr std::string_view kKeyValueSeparator = ":";
constexpr std::string_view kLineSeparator = "\n";

}  // namespace

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

bool Settings::HasKey(const std::string& key) const noexcept {
  auto it = storage_.find(key);
  return it != storage_.cend();
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

Settings LoadSettingsFromText(const std::string& text) {
  Settings storage;
  const char* savelptr = nullptr;
  for (auto line = internal::StrToken(text, kLineSeparator, &savelptr);
       savelptr; line = internal::StrToken(text, kLineSeparator, &savelptr)) {
    auto pos = line.find(kKeyValueSeparator);
    if (pos == std::string_view::npos) {
      assert(false && "key-value is invalid in settings.");
      continue;
    }
    auto key = internal::StrStrip(line.substr(0, pos++));
    auto value = internal::StrStrip(line.substr(pos));
    storage.Put(std::string{key.data(), key.size()},
                std::string{value.data(), value.size()});
  }
  return storage;
}

std::string SaveSettingsToText(const Settings& settings) {
  std::string text;
  text.reserve(kSettingsCapacity);
  auto first = settings.storage_.cbegin();
  auto last = settings.storage_.cend();
  for (auto it = first; it != last; ++it) {
    if (it != first) {
      text += kLineSeparator;
    }
    text += it->first;
    text += kKeyValueSeparator;
    text += " ";
    text += it->second;
  }
  return text;
}

Settings LoadSettingsFromFile(
    const std::filesystem::path& path,
    std::function<std::string(const std::string&)> decode) {
  std::ifstream in(path, std::ifstream::ate);
  if (!in) {
    auto ec = std::make_error_code(std::errc::io_error);
    throw std::system_error{ec, "open \"" + path.string() + "\" failed"};
  }
  auto size = in.tellg();
  in.seekg(0);

  std::string encrypttext(size, '\0');
  if (!in.read(&encrypttext[0], size)) {
    auto ec = std::make_error_code(std::errc::io_error);
    throw std::system_error{ec, "read \"" + path.string() + "\" error"};
  }
  return LoadSettingsFromText(decode(encrypttext));
}

void SaveSettingsToFile(const std::filesystem::path& path,
                        const Settings& settings,
                        std::function<std::string(const std::string&)> encode) {
  auto encrypttext = encode(SaveSettingsToText(settings));
  std::ofstream out(path, std::ofstream::out);
  if (!out) {
    auto ec = std::make_error_code(std::errc::io_error);
    throw std::system_error{ec, "open \"" + path.string() + "\" failed"};
  }
  if (!out.write(encrypttext.data(), encrypttext.size())) {
    auto ec = std::make_error_code(std::errc::io_error);
    throw std::system_error{ec, "write \"" + path.string() + "\" error"};
  }
}

}  // namespace fingerprint
