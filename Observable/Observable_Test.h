#ifndef OBSERVABLE_TEST_H
#define OBSERVABLE_TEST_H

#include <iostream>
#include <sstream>
#include <string>

#include "Observable.h"

namespace Observable_Test
{

// Simple function to simulate assertions
void Assert(bool condition, const std::string& message) 
{
    if (!condition) 
    {
        std::cout << "Test failed: " << message << std::endl;
    } 
    else 
    {
        std::cout << "Test passed" << std::endl;
    }
}

class TestClass 
{
public:
    void callback(int value) 
    {
        lastValue = value;
        ++count;
    }
    int lastValue = 0;
    int count = 0;
};

int total = 0;

void TestFreeFunction(int value)
{
    total += value;
}

void TestObserverRegistrationAndInvocation() 
{
    Observable<int> observable;
    TestClass instance;
    auto observer = observable.CreateObserver(&instance, &TestClass::callback);
    observable(123);

    Assert(instance.lastValue == 123, "Observer did not receive the correct value on invocation.");
}

void TestObserverFreeFunction() 
{
    Observable<int> observable;
    auto observer = observable.CreateObserver( TestFreeFunction );

    total = 0;
    observable(123);

    Assert(total == 123, "free function was not called.");
}

void TestObserverLambdaFunction() 
{
    Observable<int> observable;
    int total = 0;
    auto observer = observable.CreateObserver(
        [&total](int value){
            total += value;
        });
    observable(123);

    Assert(total == 123, "lambda function was not called.");
}

void TestObserverDestructionRemoval() 
{
    Observable<int> observable;
    TestClass instance;
    {
        auto observer = observable.CreateObserver(&instance, &TestClass::callback);
    }

    instance.lastValue = 0; // Resetting to ensure that no callback changes this value
    observable(456);

    Assert(instance.lastValue == 0, "Observer was not properly removed after destruction.");
}

void TestObserverDisabled() 
{
    TestClass instance;
    Observable<int> observable;
    auto observer1 = observable.CreateObserver(&instance, &TestClass::callback);
    auto observer2 = observable.CreateObserver(&instance, &TestClass::callback); 
    observer1.SetEnabled(false);
    // reset count
    instance.count = 0;
    observable(1);
    // only one observer should be invoked
    int count1 = instance.count;
    // reset count
    instance.count = 0;
    observer1.SetEnabled(true); 
    observable(1);
    // both observers should be invoked
    int count2 = instance.count;

    Assert(count1 == 1 && count2 == 2, "Observer was not properly disabled.");
}

void TestMultipleObservers() 
{
    Observable<int> observable;
    TestClass instance1, instance2;
    auto obs1 = observable.CreateObserver(&instance1, &TestClass::callback);
    auto obs2 = observable.CreateObserver(&instance2, &TestClass::callback);
    observable(789);

    Assert(instance1.lastValue == 789 && instance2.lastValue == 789, "Not all observers were notified correctly.");
}

void TestMultipleObservables() 
{
    Observable<int> observable1;
    Observable<int> observable2;
    TestClass instance;
    instance.count = 0;
    auto obs1 = observable1.CreateObserver(&instance, &TestClass::callback);
    auto obs2 = observable2.CreateObserver(&instance, &TestClass::callback);
    observable1(789);

    Assert(instance.count == 1, "Not all observables were properly created.");
}

void TestDifferentArguments() 
{
    class MultiArgClass {
    public:
        void callback(int a, double b, std::string c) 
        {
            lastInt = a;
            lastDouble = b;
            lastString = c;
        }
        int lastInt = 0;
        double lastDouble = 0.0;
        std::string lastString;
    };

    MultiArgClass instance;

    Observable<int, double, std::string> observable;
    auto obs = observable.CreateObserver(&instance, &MultiArgClass::callback);

    observable(10, 3.14, "test");

    Assert(instance.lastInt == 10 && instance.lastDouble == 3.14 && instance.lastString == "test",
           "Multi-argument observer did not receive correct values.");
}

void TestNoArguments() 
{
    class NoArgClass 
    {
    public:
        void callback() 
        {
            ++ count;
        }
       int count = 0;
    };

    NoArgClass instance;

    Observable observable;
    auto obs = observable.CreateObserver(&instance, &NoArgClass::callback);

    observable();

    Assert(instance.count == 1,
           "no-argument observer was not called.");
}

void RunTests() 
{
    std::cout << "Running Observable Tests\n";
    
    TestObserverRegistrationAndInvocation();
    TestObserverDestructionRemoval();
    TestMultipleObservers();
    TestDifferentArguments();
    TestObserverDisabled();
    TestMultipleObservables();
    TestObserverFreeFunction();
    TestObserverLambdaFunction();
    TestNoArguments();
}

} // namespace Observable_Test

#endif // OBSERVABLE_TEST_H
