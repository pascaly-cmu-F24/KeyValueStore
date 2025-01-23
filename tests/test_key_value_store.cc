#include "kvs/key_value_store.h"
#include <gmock/gmock.h> // For EXPECT_THAT and matchers
#include <gtest/gtest.h>

// Test fixture for KeyValueStore tests.
class KeyValueStoreTest : public ::testing::Test {
protected:
  KeyValueStore<std::string> *store;

  void SetUp() override { store = new KeyValueStore<std::string>(); }

  void TearDown() override { delete store; }
};

// Test case: Check if Get and Set work correctly.
TEST_F(KeyValueStoreTest, GetAndSet) {
  EXPECT_FALSE(store->Get("Justin").has_value());
  store->Set("Justin", "23");
  EXPECT_EQ(store->Count(), 1U);
  EXPECT_EQ(store->Get("Justin").value(), "23");
}

// Test case: Check if overwrite works.
TEST_F(KeyValueStoreTest, Overwrite) {
  store->Set("Justin", "23");
  store->Set("Justin", "24");
  EXPECT_EQ(store->Count(), 1U);
  EXPECT_EQ(store->Get("Justin").value(), "24");
}

// Test case: Check if delete works.
TEST_F(KeyValueStoreTest, Delete) {
  store->Set("Justin", "23");
  store->Del("Justin");
  EXPECT_FALSE(store->Get("Justin").has_value());

  store->Set("Justin", "25");
  EXPECT_TRUE(store->Get("Justin").has_value());
  EXPECT_EQ(store->Get("Justin").value(), "25");
}

// Test case: make sure delete is idempotent
TEST_F(KeyValueStoreTest, Delete2) {
  store->Del("Justin");
  store->Del("Justin");
  store->Del("Justin");
  EXPECT_EQ(store->Count(), 0U);
  EXPECT_FALSE(store->Get("Justin").has_value());

  store->Set("Justin", "25");
  store->Del("Justin");
  store->Del("Justin");
  EXPECT_EQ(store->Count(), 0U);
  EXPECT_FALSE(store->Get("Justin").has_value());
}

TEST_F(KeyValueStoreTest, KeysWithSpecificValue) {
  store->Set("Justin", "25");
  store->Set("Justina", "24");
  store->Set("Justine", "25");

  // Expect keys with value 42
  EXPECT_THAT(store->Keys("25"),
              ::testing::UnorderedElementsAre("Justin", "Justine"));
}

TEST_F(KeyValueStoreTest, KeysDefault) {
  store->Set("Justin", "25");
  store->Set("Justina", "24");
  store->Set("Justine", "25");

  // Expect keys with value 42
  EXPECT_THAT(store->Keys(),
              ::testing::UnorderedElementsAre("Justin", "Justine", "Justina"));
}

TEST_F(KeyValueStoreTest, Count) {
  KeyValueStore<int> *istore = new KeyValueStore<int>();
  uint32_t num_keys = 100;
  for (uint32_t i = 0; i < num_keys; i++) {
    istore->Set(std::to_string(i), i);
  }
  EXPECT_EQ(istore->Count(), num_keys);

  // same number of keys
  for (uint32_t i = 0; i < num_keys; i++) {
    istore->Set(std::to_string(i), i * 10);
  }
  EXPECT_EQ(istore->Count(), num_keys);
}

// Test case: Check if rollback works.
TEST_F(KeyValueStoreTest, Rollback) {
  store->Set("Justin", "25");
  store->Begin();
  store->Set("Justin", "24");
  EXPECT_EQ(store->Get("Justin").value(), "24");

  store->Rollback();
  EXPECT_EQ(store->Get("Justin").value(), "25");

  store->Rollback();
}

// Test case: Check if commit works.
TEST_F(KeyValueStoreTest, Commit) {
  store->Begin();
  store->Set("Joe Momma", "69");
  EXPECT_EQ(store->Get("Joe Momma").value(), "69");

  store->Commit();
  store->Rollback();
  EXPECT_EQ(store->Get("Joe Momma").value(), "69");
}

// Test case: Check if nested transactions work.
TEST_F(KeyValueStoreTest, NestedTransactions) {
  // Make sure GET reflects current uncommitted state
  store->Set("Joe Momma", "69");
  store->Begin();
  store->Del("Joe Momma");
  EXPECT_FALSE(store->Get("Joe Momma").has_value());

  // Make sure GET reflects current uncommitted state
  store->Begin();
  store->Set("Justin", "23");
  EXPECT_EQ(store->Get("Justin").value(), "23");

  // Make sure rollback works twice
  store->Rollback();
  store->Rollback();
  EXPECT_EQ(store->Get("Joe Momma").value(), "69");
  EXPECT_FALSE(store->Get("Justin").has_value());

  store->Begin();
  store->Set("Jenna Tools", "20");
  EXPECT_EQ(store->Get("Jenna Tools").value(), "20");

  // Make sure commit works
  store->Begin();
  store->Set("Jenna Tools", "21");
  store->Begin();
  store->Set("Justin", "24");
  store->Del("Jenna Tools");
  store->Rollback();
  store->Commit();
  EXPECT_EQ(store->Get("Jenna Tools").value(), "21");
  EXPECT_FALSE(store->Get("Justin").has_value());
}

TEST_F(KeyValueStoreTest, IntegerTest) {
  KeyValueStore<int> *istore = new KeyValueStore<int>();
  istore->Begin();
  istore->Set("Alice", 1);
  istore->Set("Bob", 1);
  istore->Rollback();
  istore->Commit();
  EXPECT_FALSE(istore->Get("Alice").has_value());
  EXPECT_FALSE(istore->Get("Bob").has_value());

  istore->Set("Alice", 0);
  istore->Set("Bob", 0);
  istore->Del("Bob");
  EXPECT_TRUE(istore->Get("Alice").has_value());
  EXPECT_EQ(istore->Get("Alice"), 0);
  EXPECT_FALSE(istore->Get("Bob").has_value());
  delete istore;
}

TEST_F(KeyValueStoreTest, FloatTest) {
  KeyValueStore<float> *fstore = new KeyValueStore<float>();
  fstore->Begin();
  fstore->Set("Alice", 1.0);
  fstore->Set("Bob", 1.0);
  fstore->Rollback();
  fstore->Commit();
  EXPECT_FALSE(fstore->Get("Alice").has_value());
  EXPECT_FALSE(fstore->Get("Bob").has_value());

  fstore->Set("Alice", 0.0);
  fstore->Set("Bob", 0.0);
  fstore->Del("Bob");
  EXPECT_TRUE(fstore->Get("Alice").has_value());
  EXPECT_EQ(fstore->Get("Alice"), 0.0);
  EXPECT_FALSE(fstore->Get("Bob").has_value());
  delete fstore;
}
