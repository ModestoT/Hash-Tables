#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
  Hash table key/value pair with linked list pointer.

  Note that an instance of `LinkedPair` is also a node in a linked list.
  More specifically, the `next` field is a pointer pointing to the the 
  next `LinkedPair` in the list of `LinkedPair` nodes. 
 */
typedef struct LinkedPair {
  char *key;
  char *value;
  struct LinkedPair *next;
} LinkedPair;

/*
  Hash table with linked pairs.
 */
typedef struct HashTable {
  int capacity;
  LinkedPair **storage;
} HashTable;

/*
  Create a key/value linked pair to be stored in the hash table.
 */
LinkedPair *create_pair(char *key, char *value)
{
  LinkedPair *pair = malloc(sizeof(LinkedPair));
  pair->key = strdup(key);
  pair->value = strdup(value);
  pair->next = NULL;

  return pair;
}

/*
  Use this function to safely destroy a hashtable pair.
 */
void destroy_pair(LinkedPair *pair)
{
  if (pair != NULL) {
    free(pair->key);
    free(pair->value);
    free(pair);
  }
}

/*
  djb2 hash function

  Do not modify this!
 */
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

/*
  Fill this in.

  All values in storage should be initialized to NULL
 */
HashTable *create_hash_table(int capacity)
{
  HashTable *ht = malloc(sizeof(HashTable));

  // initialized all values in the storage as NULL using calloc()
  ht->storage = calloc(capacity, sizeof(LinkedPair *)); 
  ht->capacity = capacity; 

  return ht;
}

/*
  Fill this in.

  Inserting values to the same index with different keys should be
  added to the corresponding LinkedPair list.

  Inserting values to the same index with existing keys can overwrite
  the value in th existing LinkedPair list.
 */
void hash_table_insert(HashTable *ht, char *key, char *value)
{
  // hash the key to see which bucket the key value pair will be placed into
  int index = hash(key, ht->capacity);
  // check if the bucket is NULL or not
  if(ht->storage[index] != NULL){
    // if it is not NULL we then need to check if the key is the same or not
    if(strcmp(ht->storage[index]-> key, key) == 0){
      // if strcmp returns 0 the keys are the same string
      // which means we can overwrite the value at this index
      ht->storage[index]->value = value;
    } else {
      // else the keys are different and it should be added to the linked list at that index
      LinkedPair *pair = create_pair(key, value);
      // check if there is a linked list already established at this bucket
      // if not add the new pair to the linked list at this bucket
      if (ht->storage[index]->next == NULL){
        ht->storage[index]->next = pair;
      // else travel through the linked list until you get to a empty spot or find the same key
      } else {
        LinkedPair *nextPair = ht->storage[index]->next;
        while(nextPair != NULL){
          // printf("Value: %s\n", nextPair->value);
          if (strcmp(nextPair->key, key) == 0){
            nextPair->value = value;
            break;
          }
          if (nextPair->next == NULL){
            // printf("Next value is null\n");
            nextPair->next = pair;
            break;
          }
          nextPair = nextPair->next;
        }
      }
    } 
  } else {
    // if the value is NULL create a new pair to be put into the empty bucket
    LinkedPair *pair = create_pair(key, value);
    ht->storage[index] = pair;
  }
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys and remove matching LinkedPairs safely.

  Don't forget to free any malloc'ed memory!
 */
void hash_table_remove(HashTable *ht, char *key)
{
  // hash the passed in key to see which index we need to check
  int lookUpIndex = hash(key, ht->capacity);

  // check that index to see if it's NULL or not
  if(ht->storage[lookUpIndex] != NULL){
    // if it's not NULL see if the key is the same as the passed in key
    if(strcmp(ht->storage[lookUpIndex]->key, key) == 0){
      destroy_pair(ht->storage[lookUpIndex]);
      ht->storage[lookUpIndex] = NULL;
      return;
    // else if it's not the same key check if there is a linked Pair
    } else if(ht->storage[lookUpIndex]->next == NULL){
      //if there's not return NULL
      fprintf(stderr, "Unable to find key: %s \n", key);
      return;
    } else {
      // if the index pair has something in next, loop through the linked list to find the key
      LinkedPair *currentPair = ht->storage[lookUpIndex];
      LinkedPair *nextPair = currentPair->next;
     
      while(nextPair != NULL){
        if (strcmp(nextPair->key, key) == 0){
          // if the next pair is the correct key set the previous pair's next value as the nextPairs next value
          currentPair->next = nextPair->next;
          destroy_pair(nextPair);
          return;
        }
        nextPair = nextPair->next;
      }
    }
  } 
  // else couldn't find the key return NULL
  fprintf(stderr, "Unable to find key: %s\n", key);
  return;
}

/*
  Fill this in.

  Should search the entire list of LinkedPairs for existing
  keys.

  Return NULL if the key is not found.
 */
char *hash_table_retrieve(HashTable *ht, char *key)
{
  // hash the passed in key to see which index we need to check
  int lookUpIndex = hash(key, ht->capacity);

  // check that index to see if it's NULL or not
  if(ht->storage[lookUpIndex] != NULL){
    // if it's not NULL see if the key is the same as the passed in key
    if(strcmp(ht->storage[lookUpIndex]->key, key) == 0){
      return ht->storage[lookUpIndex]->value;

    // else if it's not the same key check if there is a linked Pair
    } else if(ht->storage[lookUpIndex]->next == NULL){
      //if there's not return NULL
      fprintf(stderr, "Unable to find key: %s \n", key);
      return NULL;
    } else {
      // if the index pair has something in next, loop through the linked list to find the key
      LinkedPair *nextPair = ht->storage[lookUpIndex]->next;
     
      while(nextPair != NULL){
        if (strcmp(nextPair->key, key) == 0){
          return nextPair->value;
        }
        nextPair = nextPair->next;
      }
    }
  } 
  // else couldn't find the key return NULL
  fprintf(stderr, "Unable to find key: %s\n", key);
  return NULL;
}

/*
  Fill this in.

  Don't forget to free any malloc'ed memory!
 */
void destroy_hash_table(HashTable *ht)
{
  // free all the linked pairs in storage
  for (int i = 0; i < ht->capacity; i++){
    LinkedPair *nextPair = ht->storage[i]->next;
    if(nextPair != NULL){
      LinkedPair *temp = nextPair;
      while(nextPair != NULL){
        destroy_pair(nextPair);
        nextPair = temp->next;
      }
    }
    destroy_pair(ht->storage[i]);
  }

  // free the hash table
  free(ht->storage);
  free(ht);
}

/*
  Fill this in.

  Should create a new hash table with double the capacity
  of the original and copy all elements into the new hash table.

  Don't forget to free any malloc'ed memory!
 */
HashTable *hash_table_resize(HashTable *ht)
{
  // initalize new capacity and hash table
  int newCap = ht->capacity*2;
  HashTable *new_ht = malloc(sizeof(HashTable));

  new_ht->capacity = newCap;
  new_ht->storage = calloc(newCap, sizeof(LinkedPair *));
  
  // copy over values from old hash table to the new hash table
  for(int i = 0; i < ht->capacity; i++){
    // check if current index value is not null
    if(ht->storage[i] != NULL){
      // copy over the pair from the old storage to new one
      // int newHash = hash(ht->storage[i]->key, new_ht->capacity);
      // new_ht->storage[newHash] = ht->storage[i];
      hash_table_insert(new_ht, ht->storage[i]->key, ht->storage[i]->value);
      // then check if there is a linked list
      if(ht->storage[i]->next != NULL){
        LinkedPair *nextPair = ht->storage[i]->next;
        while(nextPair != NULL){
          hash_table_insert(new_ht, nextPair->key, nextPair->value);
          nextPair = nextPair->next;
        }
      }
    } 
  }
  // after copying the pairs over destory the old table
  destroy_hash_table(ht);
  return new_ht;
}


#ifndef TESTING
int main(void)
{
  struct HashTable *ht = create_hash_table(8);

  hash_table_insert(ht, "key-0", "val-0");
  hash_table_insert(ht, "key-1", "val-1");
  hash_table_insert(ht, "key-2", "val-2");
  hash_table_insert(ht, "key-3", "val-3");
  hash_table_insert(ht, "key-4", "val-4");
  hash_table_insert(ht, "key-5", "val-5");
  hash_table_insert(ht, "key-6", "val-6");
  hash_table_insert(ht, "key-7", "val-7");
  hash_table_insert(ht, "key-8", "val-8");
  hash_table_insert(ht, "key-9", "val-9");
  hash_table_remove(ht, "key-9");
  // printf("found:%s\n", hash_table_retrieve(ht, "key-0"));
  // printf("%s\n", hash_table_retrieve(ht, "key-1"));
  // printf("%s\n", hash_table_retrieve(ht, "key-2"));
  // printf("found:%s\n", hash_table_retrieve(ht, "key-3"));
  // printf("%s\n", hash_table_retrieve(ht, "key-4"));
  // printf("%s\n", hash_table_retrieve(ht, "key-5"));
  // printf("found:%s\n", hash_table_retrieve(ht, "key-6"));
  // printf("%s\n", hash_table_retrieve(ht, "key-7"));
  // printf("%s\n", hash_table_retrieve(ht, "key-8"));
  // printf("%s\n", hash_table_retrieve(ht, "key-9"));

  int old_capacity = ht->capacity;
  ht = hash_table_resize(ht);
  int new_capacity = ht->capacity;

  printf("\nResizing hash table from %d to %d.\n", old_capacity, new_capacity);

  destroy_hash_table(ht);

  return 0;
}
#endif
