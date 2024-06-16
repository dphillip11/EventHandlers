## Comparison of Event Management Implementations

**Overview:**
There are two implementations of event callback management `Observable` && `CallbackRegistry`.
The first relies upon the observer to manage it's callback and the second registers the callback with with the registry.

### Observable

**Description:**
The `Observable` class functions as a core part of the observer design pattern, where it allows multiple observer objects to subscribe to and receive notifications from the observable object when certain events occur. The observers hold and manage the scope of their callbacks. 

**Pros:**
- **Automatic lifetime:** The `Observer` must be in scope for the callback to be effective.

**Cons:**
- **Overhead of Observer Objects:** The management of observer objects may introduce overhead in terms of memory and processing.

### CallbackRegistry (Event using Hashes)

**Description:**
`CallbackRegistry` leverages hash values as unique identifiers for function callbacks, ensuring that subscriptions are uniquely and efficiently managed. This system is particularly useful in environments where compile-time safety and efficiency are paramount.

**Pros:**
- **Unique Identifier Mapping:** Provides a clear and consistent method to identify each callback, simplifying management.

**Cons:**
- **Risk of Hash Collisions:** Although minimal with a good hash function, there is a theoretical risk of hash collisions.

Each implementation has its own strengths and weaknesses, and the choice between them should be based on the specific requirements of the project, such as the number of callbacks, the need for fine-grained control, and the importance of compile-time safety.
