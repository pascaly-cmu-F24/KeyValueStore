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
#include <unordered_map>
#include <iostream>

using namespace std;

/**
 * @ brief the command struct for the transaction.
 */

template <typename T>
struct Command
{
  string command;
  string key;
  optional<T> value;

  void display()
  {
    cout << command << " " << key << " ";
    if (value.has_value())
    {
      cout << value.value() << endl;
    }
    else
    {
      cout << "NO VALUE" << endl;
    }
  }
};

/**
 * @brief The Key Value Store implementation of an In Memory DB.
 */
template <typename T>
class KeyValueStore : public InMemoryStore<T>
{

private:
  unordered_map<string, T> store;
  vector<Command<T>> commands;

public:
  /**
   * @brief Constructor.
   */
  KeyValueStore() {}

  /**
   * @brief Destructor.
   */
  ~KeyValueStore() override
  {
  }

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
  void SnapShot(string_view key);
  void Begin();
  void Commit();
  void Rollback();
};

/** ---------- MAIN PUBLIC METHODS ---------- */

template <typename T>
void KeyValueStore<T>::SnapShot(string_view key)
{
  auto where = store.find(string(key));
  if (where != store.end())
  {
    commands.push_back(Command<T>{"whatever", where->first, where->second});
  }
  else
  {
    commands.push_back(Command<T>{"whatever", string(key), nullopt});
  }
}

template <typename T>
std::optional<T> KeyValueStore<T>::Get(std::string_view key) const
{
  auto iter = store.find(string(key));
  if (iter != store.end())
  {
    return iter->second;
  }
  else
  {
    return nullopt;
  }
}

template <typename T>
void KeyValueStore<T>::Set(std::string_view key, const T &value)
{
  SnapShot(key);
  store[string(key)] = value;
}

template <typename T>
void KeyValueStore<T>::Del(std::string_view key)
{
  SnapShot(key);
  store.erase(string(key));
}

template <typename T>
void KeyValueStore<T>::Begin()
{
  commands.push_back(
      Command<T>{"Begin", "", nullopt});
}

template <typename T>
void KeyValueStore<T>::Commit()
{
  commands.clear();
}

template <typename T>
void KeyValueStore<T>::Rollback()
{
  while (!commands.empty())
  {
    Command<T> command = commands.back();
    commands.pop_back();
    if (command.command == "Begin")
    {
      break;
    }
    else if (command.value.has_value())
    {
      store[command.key] = command.value.value();
    }
    else
    {
      store.erase(command.key);
    }
  }
}

template <typename T>
std::vector<std::string>
KeyValueStore<T>::Keys(std::optional<T> with_value) const
{
  vector<string> keys;
  for (auto &[key, val] : store)
  {
    if (val == with_value.value_or(val))
    {
      keys.push_back(key);
    }
  }
  return keys;
}

template <typename T>
std::vector<T> KeyValueStore<T>::Values() const
{
  vector<T> values;
  for (auto &[key, val] : store)
  {
    values.push_back(val);
  }
  return values;
}

template <typename T>
void KeyValueStore<T>::Show(uint32_t max_records) const
{
  uint32_t count = 0;
  for (auto &[key, val] : store)
  {
    cout << key << " : " << val << endl;
    if (count++ >= max_records)
      break;
  }

  for (auto command : commands)
  {
    command.display();
  }
}

template <typename T>
uint32_t KeyValueStore<T>::Count(std::optional<T> with_value) const
{

  uint32_t size = 0;
  for (auto &[key, val] : store)
  {
    if (val == with_value.value_or(val))
    {
      size++;
    }
  }
  return size;
}

#endif