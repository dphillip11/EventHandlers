#ifndef OBSERVABLE_IMPL_H
#define OBSERVABLE_IMPL_H

#include "Observable.h"

template <typename... Args>
Observable<Args...>::~Observable()
{
    for (Observer* observer : _observers)
    {
        if (observer)
        {
            observer->_observable = nullptr;
        }
    }
}

template <typename... Args>
void Observable<Args...>::operator()(Args... args)
{
    for (Observer* observer : _observers)
    {
        (*observer)(args...);
    }
}

template <typename... Args>
template <typename Class>
typename Observable<Args...>::Observer Observable<Args...>::CreateObserver(Class* instance, void (Class::*callback_raw)(Args...))
{
    if (instance && callback_raw)
    {
        auto callback = [instance, callback_raw](Args... args) {
            (instance->*callback_raw)(args...);
        };

        return Observer(callback, this);
    }
    return Observer(nullptr, this);
}

template <typename... Args>
typename Observable<Args...>::Observer Observable<Args...>::CreateObserver(std::function<void(Args...)> callback)
{
    return Observer(callback, this);
}

template <typename... Args>
Observable<Args...>::Observer::Observer(std::function<void(Args...)> callback, Observable<Args...>* observable)
    : _callback(callback), _observable(observable)
{
    if (_observable)
    {
        _observable->_observers.insert(this);
    }
}

template <typename... Args>
Observable<Args...>::Observer::~Observer()
{
    if (_observable)
    {
        _observable->_observers.erase(this);
    }
}

template <typename... Args>
void Observable<Args...>::Observer::SetEnabled(bool enabled)
{
    _enabled = enabled;
}

template <typename... Args>
bool Observable<Args...>::Observer::IsValid() const
{
    return _callback && _observable;
}

template <typename... Args>
void Observable<Args...>::Observer::operator()(Args... args)
{
    if (_callback && _enabled)
    {
        _callback(args...);
    }
}

#endif
