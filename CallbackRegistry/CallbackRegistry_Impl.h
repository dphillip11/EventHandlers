#ifndef CALLBACK_REGISTRY_IMPL_H
#define CALLBACK_REGISTRY_IMPL_H

#include "CallbackRegistry.h"

template<typename... Args>
void CallbackRegistry<Args...>::Register(const char* identifier, Callback callback)
{
    if (_subscribers.find(identifier) != _subscribers.end())
    {
        assert(false);  // Possible reuse of the same identifier or hash collision
        return;
    }
    _subscribers[identifier] = CallbackEntry(std::move(callback));
}

template<typename... Args>
template<typename Class>
void CallbackRegistry<Args...>::Register(const char* identifier, Class* instance, void (Class::*callback)(Args...))
{
    Register(identifier, [instance, callback](Args... args) { (instance->*callback)(args...); });
}

template<typename... Args>
bool CallbackRegistry<Args...>::Unregister(const char* identifier)
{
    return _subscribers.erase(identifier) > 0;
}

template<typename... Args>
void CallbackRegistry<Args...>::SetEnabled(const char* identifier, bool enabled)
{
    auto it = _subscribers.find(identifier);
    if (it != _subscribers.end())
    {
        it->second._enabled = enabled;
    }
    else
    {
        assert(false);  // Identifier not found
    }
}

template<typename... Args>
void CallbackRegistry<Args...>::Invoke(const Args&... args) const
{
    for (const auto& [key, entry] : _subscribers)
    {
        if (entry._enabled) {
            entry._callback(args...);
        }
    }
}

template<typename... Args>
struct CallbackRegistry<Args...>::StringHash 
{
    std::size_t operator()(const char* s) const 
    {
        std::size_t hash = 5381;
        while (*s) 
        {
            hash = ((hash << 5) + hash) + *s++;
        }
        return hash;
    }
};

template<typename... Args>
struct CallbackRegistry<Args...>::StringEqual 
{
    bool operator()(const char* s1, const char* s2) const 
    {
      return std::strcmp(s1, s2) == 0;
    }
};

template<typename... Args>
struct CallbackRegistry<Args...>::CallbackEntry
{
    Callback _callback;
    bool _enabled;

    CallbackEntry() : _enabled(true) {}  // Default constructor
    CallbackEntry(Callback callback, bool enabled = true)
    : _callback(std::move(callback)), _enabled(enabled) {}

    void operator()(Args... args)
    {
        if (_enabled && _callback) {
            _callback(args...);
        }
    }
};

#endif // CALLBACK_REGISTRY_IMPL_H