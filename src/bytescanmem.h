/*******************************
bytescanmem.h
Manage data structures for bytescan stats
 ******************************/

/*****************************
----- ELEMENTS -----

KEY
a key is an n-bytes word. The program's aim is to identify each possible key and gather
information about its occurrences:
- number of occurrences
- position of each occurrence 
Depending on key's size and file's size, many keys may be gathered during a file scan.
That's why a dynamic memory allocation may be preferrable.
Key size in bytes should be a program's input.

DICTIONARY
a dictionary is a collection of items, and it's compiled during file analysis.
By means of the dictionary, key's statistics can be accessed and updated

ITEM
An item is the group of a key and its stats. Items compose a dictionary.

STATS
key's stats are of two types: occurrence count and occurrence position. The count is
a single attribute of the key, while occurrences are a collection associated with the 
key.
 ****************************/

typedef struct {
} key_t;

typedef struct {
} occurrence_count_t;

typedef struct {
} occurrence_position_t;

typedef struct {
  key_t * key;
  occurrence_count_t * count;
  occurrence_position_t * occurs;
} item_t;

typedef struct {
  item_t ** items;
} dictionary_t;


int addkey (dictionary_t * dictionary, key_t * key);
int upcount (key_t * key);
