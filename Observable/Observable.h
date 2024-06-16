#ifndef OBSERVABLE_H
#define OBSERVABLE_H

#include <functional>
#include <set>

// this class allows a function call with a set of parameters to be 
// delegated to a set of observers
template <typename...Args>
class Observable
{
public:
    class Observer;

    Observable() = default;
    ~Observable();

    void operator()(Args... args);

    // Observer constructor, syntactic sugar for member function callbacks
    // Example Usage: auto observer = observable.CreateObserver(&instance, &Class::Callback);
    template <typename Class>
    [[nodiscard]] Observer CreateObserver(Class* instance, void (Class::*callback_raw)(Args...));
    // Observer constructor
    [[nodiscard]] Observer CreateObserver(std::function<void(Args...)> callback);

    // The observer owns its callback function and manages its scope
    class Observer
    {
        friend class Observable<Args...>;

    public:
        ~Observer();
        // this toggle allows the callback to be skipped
        void SetEnabled(bool enabled);
        // Check that this observer observes an in scope event and has a valid callback
        bool IsValid() const;

    private:
        // Should only be created by an observable
        Observer(std::function<void(Args...)> callback, Observable<Args...>* observable);
        // Should only called via an observable function call
        void operator()(Args... args);

        std::function<void(Args...)> _callback;
        bool _enabled = true;
        Observable<Args...>* _observable = nullptr;
    };

private:
    std::set<Observer*> _observers;
};

#include "Observable_Impl.h"

#endif // OBSERVABLE_H