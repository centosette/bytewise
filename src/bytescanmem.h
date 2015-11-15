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

//to hold a key value. simply a pointer to an array of chars, so that key length may be
//customized
typedef struct {
  char ** keychars;
} key_t;

//occurrence counter, as a pointer to unsigned long
typedef struct {
  unsigned long * occurrences;
} occurrence_count_t;

//positions, as a linked list
typedef struct {
  unsigned long * position;
  occurrence_position_t * next;
} occurrence_position_t;


//an item collects key, occurrence count and the head of the occurrence positions list
typedef struct {
  key_t * key;
  occurrence_count_t * count;
  occurrence_position_t * occurs;
} item_t;


//a dictionary is a collection of items, implemented as a tree
typedef struct {
  item_t ** items;
} dictionary_t;


/*****************************
key managing functions
 *****************************/

key_t * create_key (char ** buf, int len);
int compare_keys (key_t * key1, key_t * key2); // returns 0 if equal; -1 if key1 is lesser than key2; 1 if key1 is greater than key 2.




int additem (dictionary_t * dictionary, key_t * key);
int upcount (key_t * key);
