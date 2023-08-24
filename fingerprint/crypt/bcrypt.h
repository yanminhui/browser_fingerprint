#ifndef BCRYPT_H
#define BCRYPT_H

#include <string>
#include <string_view>

namespace fingerprint {

constexpr unsigned gkBcryptRounds = 10u;

// The bcrypt algorithm, where the value of rounds can be between 4 and 31 and
// specifies the base 2 logarithm of the number of rounds. The special rounds
// value ‘a’ automatically selects rounds based on system performance.
std::string BcryptNewHash(std::string_view password, unsigned rounds = gkBcryptRounds);

bool BcryptCheckPass(std::string_view password, std::string_view hash);

} // namespace fingerprint

#endif  // BCRYPT_H
