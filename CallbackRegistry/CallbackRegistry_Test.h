#ifndef CALLBACK_REGISTRY_TEST_H
#define CALLBACK_REGISTRY_TEST_H

#include <iostream>
#include <sstream>
#include <string>

#include "CallbackRegistry.h"

namespace CallbackRegistry_Test
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

void TestCallbackRegistrationAndInvocation() 
{
    CallbackRegistry<int> registry;
    TestClass instance;
    registry.Register("TestClass::callback", [&instance](int value){ instance.callback(value); });
    registry.Invoke(123);

    Assert(instance.lastValue == 123, "Callback did not receive the correct value on invocation.");
}

void TestCallbackFreeFunction() 
{
    CallbackRegistry<int> registry;
    registry.Register("TestFreeFunction", TestFreeFunction);

    total = 0;
    registry.Invoke(123);

    Assert(total == 123, "Free function was not called.");
}

void TestCallbackLambdaFunction() 
{
    CallbackRegistry<int> registry;
    int lambdaTotal = 0;
    registry.Register("LambdaFunction", [&lambdaTotal](int value){ lambdaTotal += value; });
    registry.Invoke(123);

    Assert(lambdaTotal == 123, "Lambda function was not called.");
}

void TestCallbackUnregistration() 
{
    CallbackRegistry<int> registry;
    TestClass instance;
    registry.Register("TestClass::callback", [&instance](int value){ instance.callback(value); });
    bool unregistered = registry.Unregister("TestClass::callback");
    registry.Invoke(456);

    Assert(unregistered && instance.count == 0, "Callback was not properly removed after unregistration.");
}

void TestMultipleCallbacks() 
{
    CallbackRegistry<int> registry;
    TestClass instance1, instance2;
    registry.Register("Instance1::callback", [&instance1](int value){ instance1.callback(value); });
    registry.Register("Instance2::callback", [&instance2](int value){ instance2.callback(value); });
    registry.Invoke(789);

    Assert(instance1.lastValue == 789 && instance2.lastValue == 789, "Not all callbacks were notified correctly.");
}

void RunTests() 
{
    std::cout << "Running Callback Registry Tests\n";

    TestCallbackRegistrationAndInvocation();
    TestCallbackUnregistration();
    TestMultipleCallbacks();
    TestCallbackFreeFunction();
    TestCallbackLambdaFunction();
}

} // namespace CallbackRegistry_Test

#endif // CALLBACK_REGISTRY_TEST_H
