#ifndef CALLBACK_REGISTRY_H
#define CALLBACK_REGISTRY_H

#include <cassert>
#include <functional>
#include <unordered_map>
#include <cstring>

template<typename... Args>
class CallbackRegistry
{
public:
  using Callback = std::function<void(Args...)>;

  // Public interface
  void Register(const char* identifier, Callback callback);
  template<typename Class>
  void Register(const char* identifier, Class* instance, void (Class::*callback)(Args...));
  bool Unregister(const char* identifier);
  void SetEnabled(const char* identifier, bool enabled);
  void Invoke(const Args&... args) const;

private:
  // Custom hash and equality functors
  struct StringHash;
  struct StringEqual;
  struct CallbackEntry;

  // Using custom hash and equality functors
  using SubscribersMap = std::unordered_map<const char*, CallbackEntry, StringHash, StringEqual>;
  SubscribersMap _subscribers;
};

#include "CallbackRegistry_Impl.h"

#endif // CALLBACK_REGISTRY_H
