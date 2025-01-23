/**
 * This is the reference solution file for 18671 assignment 0.
 * Author: Justin Ventura (jmventur@andrew.cmu.edu)
 *
 * NOTE: Keeping template class implementation files in the header file is
 * known as the 'inclusion model' for templates. Keep in mind that doing this is
 * pretty standard, but it can hurt compilation times in large projects. For the
 * sake of the assignment, that's fine.
 */

#ifndef KEY_VALUE_STORE_H
#define KEY_VALUE_STORE_H

#include "store/in_memory_store.h"

/**
 * @brief The Key Value Store implementation of an In Memory DB.
 */
template <typename T> class KeyValueStore : public InMemoryStore<T> {
public:
  /**
   * @brief Constructor.
   */
  KeyValueStore() {}

  /**
   * @brief Destructor.
   */
  ~KeyValueStore() override {}

  /** MAIN KEY VALUE STORE INTERACTION METHODS; Must be implemented. */
  std::optional<T> Get(std::string_view key) const override;
  void Set(std::string_view key, const T &value) override;
  void Del(std::string_view key) override;

  /** ADDITIONAL METHODS; Must be implemented. */
  std::vector<std::string>
  Keys(std::optional<T> with_value = std::nullopt) const override;
  std::vector<T> Values() const override;
  void Show(uint32_t max_records = 100) const override;
  uint32_t Count(std::optional<T> with_value = std::nullopt) const override;

  /** TRANSACTION SUPPORT METHODS; Unique to this impl */
  void Begin();
  void Commit();
  void Rollback();
};

/** ---------- MAIN PUBLIC METHODS ---------- */

template <typename T>
std::optional<T> KeyValueStore<T>::Get(std::string_view key) const {
  throw std::logic_error("KeyValueStore<T>::Get not implemented.");
}

template <typename T>
void KeyValueStore<T>::Set(std::string_view key, const T &value) {
  throw std::logic_error("KeyValueStore<T>::Set not implemented.");
}

template <typename T> void KeyValueStore<T>::Del(std::string_view key) {
  throw std::logic_error("KeyValueStore<T>::Del not implemented.");
}

template <typename T> void KeyValueStore<T>::Begin() {
  throw std::logic_error("KeyValueStore<T>::Begin not implemented.");
}

template <typename T> void KeyValueStore<T>::Commit() {
  throw std::logic_error("KeyValueStore<T>::Commit not implemented.");
}

template <typename T> void KeyValueStore<T>::Rollback() {
  throw std::logic_error("KeyValueStore<T>::Rollback not implemented.");
}

template <typename T>
std::vector<std::string>
KeyValueStore<T>::Keys(std::optional<T> with_value) const {
  throw std::logic_error("KeyValueStore<T>::Keys not implemented.");
}

template <typename T> std::vector<T> KeyValueStore<T>::Values() const {
  throw std::logic_error("KeyValueStore<T>::Values not implemented.");
}

template <typename T> void KeyValueStore<T>::Show(uint32_t max_records) const {
  throw std::logic_error("KeyValueStore<T>::Show not implemented.");
}

template <typename T>
uint32_t KeyValueStore<T>::Count(std::optional<T> with_value) const {
  throw std::logic_error("KeyValueStore<T>::Count not implemented.");
}

#endif