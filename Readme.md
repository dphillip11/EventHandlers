## Comparison of Event Management Implementations

**Overview:**
There are two implementations of event callback management `Observable` && `CallbackRegistry`.
The first relies upon the observer to manage it's callback and the second registers the callback with the registry.

### Observable

**Description:**
The `Observable` class functions as part of the observer design pattern, where it delegates function calls to observers that remain in scope. The observers hold and manage the scope of their callbacks. 

**Pros:**
- **Automatic lifetime:** The `Observer` must be in scope for the callback to be effective.

**Cons:**
- **Overhead of Observer Objects:** The management of observer objects may introduce overhead in terms of memory and processing.

### CallbackRegistry (Event using Hashes)

**Description:**
`CallbackRegistry` uses hashed identifier strings as unique identifiers for function callbacks, ensuring that subscriptions are uniquely and efficiently managed. The implementation could be made more efficient by using compile time calculated or precalculated hash values instead of passing strings into the function calls.

**Pros:**
- **Unique Identifier Mapping:** Provides a clear and consistent method to identify each callback, simplifying management.

**Cons:**
- **Risk of Hash Collisions:** Although minimal with a good hash function, there is a theoretical risk of hash collisions.
