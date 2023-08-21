#pragma once

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

}  // namespace internal
}  // namespace fingerprint
