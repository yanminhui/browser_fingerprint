#include "fingerprint/utility.h"

#include <string_view>
#include <type_traits>

#include "fingerprint/internal/uniform_int_distribution.h"

namespace fingerprint {

std::string NewRandomStr(std::size_t len, unsigned char_type) {
  constexpr std::string_view kLowerAlphas = "abcdefghjkmnpqrstuvwxy";
  constexpr std::string_view kUpperAlphas = "ABCDEFGHJKLMNPQRSTUVWXY";
  constexpr std::string_view kDigits = "3456789";
  constexpr std::string_view kUnderscore = "_";
  constexpr std::string_view kHyphen = "-";
  constexpr std::string_view kSpecialCharsForPassword =
      "!#$%&()*,./:;?@[]^_{|}~+<=>";

  std::string charset;
  if (char_type & RandomChar::kLowerAlpha) {
    charset += kLowerAlphas;
  }
  if (char_type & RandomChar::kUpperAlpha) {
    charset += kUpperAlphas;
  }
  if (char_type & RandomChar::kDigit) {
    charset += kDigits;
  }
  if (char_type & RandomChar::kUnderscore) {
    charset += kUnderscore;
  }
  if (char_type & RandomChar::kHyphen) {
    charset += kHyphen;
  }
  if (char_type & RandomChar::kSpecialCharForPassword) {
    charset += kSpecialCharsForPassword;
  }
  const bool kHasHyphen = (charset.find(kHyphen) != std::string::npos);

  std::string result(len, '\0');
  internal::UniformIntDistribution<std::string_view::size_type> uid(
      0, charset.size());
  for (std::size_t i = 0; i != len;) {
    bool insert_hyphen = (((i + 1) % 7) == 0);
    if (!kHasHyphen && insert_hyphen) {
      result += kHyphen;
      ++i;
      continue;
    }

    auto ch = charset[uid()];
    if (i == 0 && (char_type & RandomChar::kUserNameRequired)) {
      if (!std::isalpha(ch)) {
        continue;
      }
    }
    result.push_back(ch);
    ++i;
  }
  return result;
}

}  // namespace fingerprint
