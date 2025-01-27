#include "kvs/key_value_store.h"
#include "store/in_memory_store.h"
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  KeyValueStore<int> *istore = new KeyValueStore<int>();

  istore->Begin();
  istore->Set("Alice", 1);
  istore->Set("Bob", 1);
  istore->Show(32);
  istore->Rollback();
  istore->Show(32);
}