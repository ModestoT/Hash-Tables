#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/****
  Basic hash table key/value pair
 ****/
typedef struct Pair {
  char *key;
  char *value;
} Pair;

/****
  Basic hash table
 ****/
typedef struct BasicHashTable {
  int capacity;
  Pair **storage;
} BasicHashTable;

/****
  Create a key/value pair to be stored in the hash table.
 ****/
Pair *create_pair(char *key, char *value)
{
  Pair *pair = malloc(sizeof(Pair));
  pair->key = strdup(key);
  pair->value = strdup(value);

  return pair;
}

/****
  Use this function to safely destroy a hashtable pair.
 ****/
void destroy_pair(Pair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/****
  djb2 hash function

  Do not modify this!
 ****/
unsigned int hash(char *str, int max)
{
  unsigned long hash = 5381;
  int c;
  unsigned char * u_str = (unsigned char *)str;

  while ((c = *u_str++)) {
    hash = ((hash << 5) + hash) + c;
  }

  return hash % max;
}


/****
  Fill this in.

  All values in storage should be initialized to NULL
  (hint: look up `calloc`)
 ****/
BasicHashTable *create_hash_table(int capacity)
{
  BasicHashTable *ht = malloc(sizeof(BasicHashTable));
  ht->storage = calloc(capacity, sizeof(Pair *)); // initialized all values in the storage as NULL using calloc()
  ht->capacity = capacity; 

  return ht;
}

/****
  Fill this in.

  If you are overwriting a value with a different key, print a warning.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_insert(BasicHashTable *ht, char *key, char *value)
{
  // hash the key to figure out the index where the pair will be placed
  int index = hash(key, ht->capacity); 
  // Create the pair using the passed in key and value
  Pair *pair = create_pair(key, value);

  if (ht->storage[index] != NULL){
    printf("Warning overwriting value at this index\n");
  } 
  // Place the pair at the hashed index
  ht->storage[index] = pair;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void hash_table_remove(BasicHashTable *ht, char *key)
{
  int index = hash(key, ht->capacity);
  // check if the key being removed is valid
  if (ht->storage[index] == NULL){
    fprintf(stderr, "Key not found\n");
    return;
  }
  destroy_pair(ht->storage[index]);
}

/****
  Fill this in.

  Should return NULL if the key is not found.
 ****/
char *hash_table_retrieve(BasicHashTable *ht, char *key)
{
  // hash the passed in key to be used to check the storage for a value
  int lookupIndex = hash(key, ht->capacity);
  // then check to see if that index is NULL or not
  if (ht->storage[lookupIndex] != NULL){
    // if it's not null compare that index's key string to the passed in key string
    if (strcmp(ht->storage[lookupIndex]->key, key) == 0){
      // if strings are the same return the value for the key
      return ht->storage[lookupIndex]->value;
    } 
  }
  
  return NULL;
}

/****
  Fill this in.

  Don't forget to free any malloc'ed memory!
 ****/
void destroy_hash_table(BasicHashTable *ht)
{
  // Free all the pairs in the storage
  for (int i = 0; i < ht->capacity; i++){
    destroy_pair(ht->storage[i]);
  }

  // Free the hasttable
  free(ht->storage);
  free(ht);
}


#ifndef TESTING
int main(void)
{
  struct BasicHashTable *ht = create_hash_table(16);

  hash_table_insert(ht, "line", "Here today...\n");

  printf("%s", hash_table_retrieve(ht, "line"));

  hash_table_remove(ht, "line");

  if (hash_table_retrieve(ht, "line") == NULL) {
    printf("...gone tomorrow. (success)\n");
  } else {
    fprintf(stderr, "ERROR: STILL HERE\n");
  }

  destroy_hash_table(ht);

  return 0;
}
#endif
