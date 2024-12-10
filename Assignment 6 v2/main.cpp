#include <iostream>
#include <vector>
#include <list>
#include <chrono>
#include <utility>//help manage and manipulate objects effectivly.
#include <string> // For std::string and std::to_string
#include <cstdlib> // For std::rand and std::srand
#include <ctime> // For std::time

// Using a namespace to prevent potential naming conflicts and to logically group
// associated functionalities.
//The outline of this project was from CODEPAL:https://codepal.ai/code-generator/query/pOKbbL43/implement-hash-table-separate-chaining-cpp
//I adjusted sections of the code so it would be able to work for this project.
//I used Stack Overflow to assist with some syntax errors. Additonally I would like to mention that Terrance Sims also helped me with fixing my return function. 
//https://stackoverflow.com/questions/68076567/on-line-138-of-my-code-i-keep-getting-an-expected-a-error-code-e0065-i
namespace HashTableTot {

    enum class CollisionMethod {
        SEPARATE_CHAINING,
        LINEAR_PROBING // Phase 2 implementation
    };

    template<typename KeyType, typename ValueType>
    class HashTable {
    private:
        struct KeyValuePair {
            KeyType key;
            ValueType value;
        };

        int numBuckets;
        std::vector<std::list<KeyValuePair>> chainTable; // For separate chaining
        std::vector<std::pair<KeyType, ValueType>> probeTable; // For linear probing
        std::vector<bool> occupied; // To track occupied slots in linear probing
        CollisionMethod collisionMethod;

        int ComputeHash(const KeyType& key) const {
            int hashValue = 0;
            for (char c : key) {
                hashValue += static_cast<int>(c);
            }
            return hashValue % numBuckets;
        }

        size_t LinearProbe(size_t index) const {
            return (index + 1) % numBuckets;
        }

    public:
        HashTable(int numBuckets, CollisionMethod method = CollisionMethod::SEPARATE_CHAINING)
            : numBuckets(numBuckets), collisionMethod(method), chainTable(numBuckets), probeTable(numBuckets), occupied(numBuckets, false) {}

        void Insert(const KeyType& key, const ValueType& value) {
            int bucketIndex = ComputeHash(key);
            if (collisionMethod == CollisionMethod::SEPARATE_CHAINING) {
                KeyValuePair pair = { key, value };
                chainTable[bucketIndex].push_back(pair);
            }
            else if (collisionMethod == CollisionMethod::LINEAR_PROBING) {
                while (occupied[bucketIndex]) {
                    if (probeTable[bucketIndex].first == key) {
                        probeTable[bucketIndex].second = value;
                        return;
                    }
                    bucketIndex = LinearProbe(bucketIndex);
                }
                probeTable[bucketIndex] = { key, value };
                occupied[bucketIndex] = true;
            }
        }

        ValueType Get(const KeyType& key) const {
            int bucketIndex = ComputeHash(key);
            if (collisionMethod == CollisionMethod::SEPARATE_CHAINING) {
                for (const KeyValuePair& pair : chainTable[bucketIndex]) {
                    if (pair.key == key) {
                        return pair.value;
                    }
                }
            }
            else if (collisionMethod == CollisionMethod::LINEAR_PROBING) {
                while (occupied[bucketIndex]) {
                    if (probeTable[bucketIndex].first == key) {
                        return probeTable[bucketIndex].second;
                    }
                    bucketIndex = LinearProbe(bucketIndex);
                }
            }
            return ValueType();
        }

        void Remove(const KeyType& key) {
            int bucketIndex = ComputeHash(key);
            if (collisionMethod == CollisionMethod::SEPARATE_CHAINING) {
                chainTable[bucketIndex].remove_if([&key](const KeyValuePair& pair)
                    {
                    return pair.key == key;
                    });
            }
            else if (collisionMethod == CollisionMethod::LINEAR_PROBING) {
                while (occupied[bucketIndex]) {
                    if (probeTable[bucketIndex].first == key) {
                        occupied[bucketIndex] = false;
                        return;
                    }
                    bucketIndex = LinearProbe(bucketIndex);
                }
            }
        }
    }; // This is the closing bracket for the HashTable class

} // This is the closing bracket for the HashTableTot namespace

// Define the PerformTest function before the main function
void PerformTest(HashTableTot::HashTable<std::string, int>& hashTable, int numOperations) {
    std::srand(std::time(nullptr)); // Seed for random number generation

    // Insert test
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; ++i) {
        int randomKey = std::rand();
        hashTable.Insert("Key" + std::to_string(randomKey), randomKey);
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;
    std::cout << "Insert time for " << numOperations << " operations: " << duration.count() << " seconds" << std::endl;

    // Retrieve test
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; ++i) {
        int randomKey = std::rand();
        hashTable.Get("Key" + std::to_string(randomKey));
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Retrieve time for " << numOperations << " operations: " << duration.count() << " seconds" << std::endl;

    // Remove test
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < numOperations; ++i) {
        int randomKey = std::rand();
        hashTable.Remove("Key" + std::to_string(randomKey));
    }
    end = std::chrono::high_resolution_clock::now();
    duration = end - start;
    std::cout << "Remove time for " << numOperations << " operations: " << duration.count() << " seconds" << std::endl;
}

int main() {
    HashTableTot::HashTable<std::string, int> hashTable(10, HashTableTot::CollisionMethod::SEPARATE_CHAINING);

    // Inserting key-value pairs into the hash table
    hashTable.Insert("One", 1);
    hashTable.Insert("Two", 2);
    hashTable.Insert("Three", 3);

    int oneCount = hashTable.Get("One");
    int twoCount = hashTable.Get("Two");
    int threeCount = hashTable.Get("Three");
    // Retrieves the Value from hash table
    std::cout << "Retrieve Key 'One': " << oneCount << std::endl;
    std::cout << "Retrieve Key 'Two': " << twoCount << std::endl;
    std::cout << "Retrieve Key 'Three': " << threeCount << std::endl;

    // Removing a key-value pair from the hash table
    hashTable.Remove("Two");

    // Trying to retrieve the value for the removed key
    int removedCount = hashTable.Get("Two");
    std::cout << "Retrieve Key 'Two' (after removal): " << removedCount << std::endl;

    // Performance tests
    PerformTest(hashTable, 100);
    PerformTest(hashTable, 1000);
    PerformTest(hashTable, 10000);

    return 0;
}