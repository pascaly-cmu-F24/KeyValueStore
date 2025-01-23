/**
 * @brief This is the Abstract Base Class for the in memory data store.
 *
 * NOTE: You may NOT modify this file.
 *
 * Author: Justin Ventura (jmventur@andrew.cmu.edu)
 */

#ifndef IN_MEMORY_STORE_H
#define IN_MEMORY_STORE_H

#include <optional>
#include <string>
#include <vector>

/**
 * @brief The InMemoryStore interface (ABC) to be implemented.
 *
 * @paragraph This in-memory storage data structure interface is designed to act
 * as a simple in-memory store that supports some useful operations such as:
 *
 * - **GET**: Returns the value of a given key in the store.
 * - **SET**: Sets a key to a value. Overwrites the value if it exists and is different.
 * - **DEL**: Deletes a given key.
 * - **KEYS**: Returns a vector of all keys in the store matching a value.
 * - **VALUES**: Returns a vector of all values in the store.
 * - **SHOW**: Prints out the store in a nicely readable format.
 * - **COUNT**: Counts the number of keys with a given value.
 *
 * You can see more details below.
 *
 * **NOTE**: Do NOT modify this.
 */
template <typename T> class InMemoryStore {
protected:
  /** @brief Keeps track of the number of unique records; the size. */
  uint32_t num_records_;

public:
  /** @brief Constructor, nothing interesting here. */
  InMemoryStore() : num_records_(0) {}

  /** @brief Destructor, note that it must be implemented. */
  virtual ~InMemoryStore() {}

  /**
   * @brief Return the value for a given key.
   * @param key a string identifier
   * @returns the corresponding value if it exists, else std::nullopt
   */
  virtual std::optional<T> Get(std::string_view key) const = 0;

  /**
   * @brief Sets the given key to the given value.
   * @param key a string identifier
   * @param value a string identifier
   *
   * NOTE: If the key exists, it will overwrite unless the given value matches
   * the existing value.
   */
  virtual void Set(std::string_view key, const T &value) = 0;

  /**
   * @brief Delete the given key from the store.
   * @param key a string identifier
   *
   * NOTE: If the key isn't in the store, just does nothing.
   */
  virtual void Del(std::string_view key) = 0;

  /**
   * @brief Return all keys with values matching the given query.
   * @param with_value the query value to be matched. Optional
   * @returns a vector of all keys matching the query
   *
   * NOTE: If with_value is std::nullopt, will just return all keys.
   */
  virtual std::vector<std::string>
  Keys(std::optional<T> with_value = std::nullopt) const = 0;

  /**
   * @brief Return all values in the store.
   * @returns a vector of all values in the store
   */
  virtual std::vector<T> Values() const = 0;

  /**
   * @brief Formats the store in a nice way. The implementer can choose however
   * they'd like to implement this. Ideally, it should be useful for whenever
   * you want to know what is in the store, potentially for debugging.
   * @param max_records the maximum number of records to print
   */
  virtual void Show(uint32_t max_records = 100) const = 0;

  /**
   * @brief Return the number of keys in the store matching the query
   * @param with_value the query value to be matched. Optional
   *
   * NOTE: If with_value is std::nullopt, will just return the total number of
   * records.
   */
  virtual uint32_t Count(std::optional<T> with_value = std::nullopt) const = 0;
};

#endif