cmake_minimum_required(VERSION 3.0.0)
project(browser_fingerprint VERSION 0.1.0 LANGUAGES C CXX)

set(sources 
  "fingerprint/crypt/base64.cc"
  "fingerprint/crypt/base64.h"
  "fingerprint/crypt/bcrypt.cc"
  "fingerprint/crypt/bcrypt.h"
  "fingerprint/crypt/blowfish.cc"
  "fingerprint/crypt/node_blf.h"
  "fingerprint/crypt/openbsd.h"
  "fingerprint/crypt/twofishcpp.cc"
  "fingerprint/crypt/twofishcpp.h"
  "fingerprint/crypt/twofish.c"
  "fingerprint/crypt/twofish.h"
  "fingerprint/fingerprint_context.cc"
  "fingerprint/fingerprint_context.h"
  "fingerprint/fingerprint_switches.h"
  "fingerprint/internal/apply_float_noise.h"
  "fingerprint/internal/uniform_float_distribution.h"
  "fingerprint/internal/uniform_int_distribution.h"
  "fingerprint/internal/string_algorithm.cc"
  "fingerprint/internal/string_algorithm.h"
  "fingerprint/services/context.cc"
  "fingerprint/services/context.h"
  "fingerprint/services/path_service.cc"
  "fingerprint/services/path_service.h"
  "fingerprint/services/settings_service.cc"
  "fingerprint/services/settings_service.h"
  "fingerprint/services/use_service.h"
  "fingerprint/settings.cc"
  "fingerprint/settings.h"
  "fingerprint/settings_keys.h"
  "fingerprint/webgl_noise.h"
  "fingerprint/webgl_renderer.cc" 
  "fingerprint/webgl_renderer.h"
  "fingerprint/utility.cc"
  "fingerprint/utility.h"
)

set(fingerprint ${PROJECT_NAME})
add_library(${fingerprint} ${sources})
target_compile_features(${fingerprint} PUBLIC cxx_std_20)
target_include_directories(${fingerprint} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# for developing
add_executable(main "fingerprint/main.cc")
target_link_libraries(main ${fingerprint})
