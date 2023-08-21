#pragma once

#include <memory>
#include <mutex>
#include <typeinfo>

#include "fingerprint/services/context.h"

namespace fingerprint {
namespace internal {

template <class ServiceT>
ServiceT& UseService(Context& ctx) {
  Context::Service* s = nullptr;
  {
    std::lock_guard<std::mutex> lock(ctx.mutex_);
    s = ctx.first_service_;
  }
  const std::type_info* id = &typeid(ServiceT);

  // return if service already exists.
  for (; s; s = s->next_) {
    if (*id == *s->id_) {
      return static_cast<ServiceT&>(*s);
    }
  }

  // create if service is not existed.
  auto new_s = std::make_unique<ServiceT>(ctx);
  new_s->id_ = id;
  {
    std::lock_guard<std::mutex> lock(ctx.mutex_);
    for (auto* ss = ctx.first_service_; ss; ss = ss->next_) {
      if (*id == *ss->id_) {
        return static_cast<ServiceT&>(*ss);
      }
    }
    new_s->next_ = ctx.first_service_;
    ctx.first_service_ = new_s.release();
    return static_cast<ServiceT&>(*ctx.first_service_);
  }
}

}  // namespace internal
}  // namespace fingerprint
