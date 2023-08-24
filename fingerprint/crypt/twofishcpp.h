#pragma once

#include <string>
#include <string_view>

namespace fingerprint {

std::string TwofishEncrypt(std::string_view plaintext, std::string_view key);
std::string TwofishDecrypt(std::string_view ciphertext, std::string_view key);

}  // namespace fingerprint
