#pragma once

#include <iterator>

#include "fingerprint/internal/apply_float_noise.h"
#include "fingerprint/internal/uniform_float_distribution.h"

namespace fingerprint {

constexpr float gkGLnoiseMinimum = 0.0001f;
constexpr float gkGLnoiseMaximum = 0.001f;
constexpr unsigned int gkGLnoiseFloatArraySize = 64u;

constexpr float gkGLclipSpaceMinimum = -1.0f;
constexpr float gkGLclipSpaceMaximum = 1.0f;

template <class FloatArray>
void GLnewNoiseFloatArray(FloatArray& out) {
  using namespace fingerprint::internal;
  out.resize(gkGLnoiseFloatArraySize);
  FillRandomFloatArray(out.begin(), out.end(), gkGLnoiseMinimum,
                       gkGLnoiseMaximum);
}

template <class Target, class Source, class Count, class NoiseFloatArray>
Target* GLapplyNoise(Target* target, const Source* source, Count count,
                     const NoiseFloatArray& noiseArray) {
  using namespace fingerprint::internal;
  ApplyFloatNoise<NoiseFloatArray> transform{gkGLclipSpaceMinimum,
                                             gkGLclipSpaceMaximum, noiseArray};
  auto first = static_cast<const Target*>(source);
  auto last = first;
  std::advance(last, count);
  transform(first, last, target);
  return target;
}

}  // namespace fingerprint
