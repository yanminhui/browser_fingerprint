cmake_minimum_required(VERSION 3.0.0)
project(browser_fingerprint VERSION 0.1.0 LANGUAGES C CXX)

set(sources 
  "fingerprint/internal/apply_float_noise.h"
  "fingerprint/internal/uniform_float_distribution.h"
  "fingerprint/internal/uniform_int_distribution.h"
  "fingerprint/internal/utility.cc"
  "fingerprint/internal/utility.h"
  "fingerprint/settings.cc"
  "fingerprint/settings.h"
  "fingerprint/webgl_noise.h"
  "fingerprint/webgl_renderer.cc" 
  "fingerprint/webgl_renderer.h"
)

set(fingerprint ${PROJECT_NAME})
add_library(${fingerprint} ${sources})
target_compile_features(${fingerprint} PUBLIC cxx_std_20)
target_include_directories(${fingerprint} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# for developing
add_executable(main "fingerprint/main.cc")
target_link_libraries(main ${fingerprint})
