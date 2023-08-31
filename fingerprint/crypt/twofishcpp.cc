#include "fingerprint/crypt/twofishcpp.h"

extern "C" {

#include "fingerprint/crypt/twofish.h"
}

#include <iostream>
namespace fingerprint {

std::string TwofishEncrypt(std::string_view plaintext, std::string_view key) {
  // initialize
  Twofish_initialise();

  // prepare dkey
  Twofish_Byte dkey[32];
  memset(dkey, 0, sizeof(dkey));

  const int dkeylen = (key.size() < sizeof(dkey) ? key.size() : sizeof(dkey));
  strncpy((char*)dkey, key.data(), dkeylen);

  Twofish_key xkey;
  Twofish_prepare_key(dkey, dkeylen, &xkey);

  // encrypt
  constexpr std::string_view::size_type blocksize = 16u;
  std::string ciphertext;
  ciphertext.reserve(plaintext.size() + (plaintext.size() % blocksize));
  for (std::string_view::size_type i = 0; i != plaintext.size();) {
    std::string_view::size_type len = plaintext.size() - i;
    if (len >= blocksize) {
      len = blocksize;
      ciphertext.resize(i + len);
      Twofish_Byte* p = (Twofish_Byte*)plaintext.data() + i;
      Twofish_Byte* c = (Twofish_Byte*)ciphertext.data() + i;
      Twofish_encrypt(&xkey, p, c);
    } else {
      ciphertext.resize(i + blocksize);
      Twofish_Byte p[blocksize];
      strncpy((char*)p, plaintext.data() + i, len);
      memset(p + len, 0, blocksize - len);
      Twofish_Byte* c = (Twofish_Byte*)ciphertext.data() + i;
      Twofish_encrypt(&xkey, p, c);
    }
    i += len;
  }
  return ciphertext;
}

std::string TwofishDecrypt(std::string_view ciphertext, std::string_view key) {
  // initialize
  Twofish_initialise();

  // prepare dkey
  Twofish_Byte dkey[32];
  memset(dkey, 0, sizeof(dkey));

  const int dkeylen = (key.size() < sizeof(dkey) ? key.size() : sizeof(dkey));
  strncpy((char*)dkey, key.data(), dkeylen);

  Twofish_key xkey;
  Twofish_prepare_key(dkey, dkeylen, &xkey);

  // decrypt
  constexpr std::string_view::size_type blocksize = 16u;
  std::string plaintext;
  plaintext.reserve(ciphertext.size() + (ciphertext.size() % blocksize));
  for (std::string_view::size_type i = 0; i != ciphertext.size();) {
    std::string_view::size_type len = ciphertext.size() - i;
    if (len >= blocksize) {
      len = blocksize;
      plaintext.resize(i + len);
      Twofish_Byte* c = (Twofish_Byte*)ciphertext.data() + i;
      Twofish_Byte* p = (Twofish_Byte*)plaintext.data() + i;
      Twofish_decrypt(&xkey, c, p);
    } else {
      plaintext.resize(i + blocksize);
      Twofish_Byte c[blocksize];
      strncpy((char*)c, ciphertext.data() + i, len);
      memset(c + len, 0, blocksize - len);
      Twofish_Byte* p = (Twofish_Byte*)plaintext.data() + i;
      Twofish_decrypt(&xkey, c, p);
    }
    i += len;
  }
  std::string::size_type n = 0;
  for (auto it = plaintext.crbegin(); it != plaintext.crend(); ++it) {
    if (*it != '\0') {
      break;
    }
    ++n;
  }
  return plaintext.substr(0, plaintext.size() - n);
}

}  // namespace fingerprint
