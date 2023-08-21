#pragma once

#include <memory>
#include <mutex>
#include <typeinfo>

namespace fingerprint {
namespace internal {

class Context {
  template <class ServiceT>
  friend ServiceT& UseService(Context& ctx);

 public:
  class Service;

 public:
  Context();
  Context(const Context&) = delete;
  Context& operator=(const Context&) = delete;
  virtual ~Context();

 private:
  void ShutdownContext();
  void DestroyContext();

 private:
  std::mutex mutex_;
  Service* first_service_;
};

class Context::Service {
  friend Context;

  template <class ServiceT>
  friend ServiceT& UseService(Context& ctx);

 protected:
  explicit Service(Context& ctx);
  virtual ~Service();

  Context& ContextRef() noexcept;

 private:
  virtual void ShutdownService();

 private:
  Context& context_;
  const std::type_info* id_;
  Service* next_;
  bool shutdown_;
};

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
    for (auto s = ctx.first_service_; s; s = s->next_) {
      if (*id == *s->id_) {
        return static_cast<ServiceT&>(*s);
      }
    }
    new_s->next_ = ctx.first_service_;
    ctx.first_service_ = new_s.release();
    return static_cast<ServiceT&>(*ctx.first_service_);
  }
}

}  // namespace internal
}  // namespace fingerprint
