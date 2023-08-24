#pragma once

#include <string>

#include "fingerprint/crypt/bcrypt.h"

namespace fingerprint {

constexpr std::size_t gkUserNameLen = 6u;
constexpr std::size_t gkGroupNameLen = 6u;
constexpr std::size_t gkPasswordLen = 20u;

using ::BcryptCheckPass;
using ::BcryptNewHash;

namespace RandomChar {

enum Type : unsigned {
  kLowerAlpha = 1 << 0,
  kUpperAlpha = 1 << 1,
  kDigit = 1 << 2,
  kUnderscore = 1 << 3,
  kHyphen = 1 << 4,
  kSpecialCharForPassword = 1 << 5,
  kUserNameRequired = kLowerAlpha | kDigit,
  kPasswordRequired = kUserNameRequired | kUpperAlpha,
};

}  // namespace RandomChar

std::string NewRandomStr(std::size_t len,
                         unsigned char_type = RandomChar::kUserNameRequired);

}  // namespace fingerprint
