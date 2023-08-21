#include "fingerprint/services/context.h"

namespace fingerprint {
namespace internal {

Context::Context() : first_service_{nullptr} {}

Context::~Context() {
  ShutdownContext();
  DestroyContext();
}

void Context::ShutdownContext() {
  Context::Service* s = first_service_;
  for (; s; s = s->next_) {
    if (!s->shutdown_) {
      s->shutdown_ = true;
      s->ShutdownService();
    }
  }
}

void Context::DestroyContext() {
  while (first_service_) {
    Service* s = first_service_->next_;
    delete first_service_;
    first_service_ = s;
  }
}

Context::Service::Service(Context& ctx)
    : context_{ctx}, id_{nullptr}, next_{nullptr}, shutdown_{false} {}

Context::Service::~Service() {}

Context& Context::Service::ContextRef() noexcept { return context_; }

void Context::Service::ShutdownService() {}

}  // namespace internal
}  // namespace fingerprint