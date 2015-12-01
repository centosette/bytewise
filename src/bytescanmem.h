

/*******************************
bytescanmem.h
Manage data structures for bytescan stats
 ******************************/
//includes

#include <stdlib.h>
#define _GNU_SOURCE
#include <string.h>
//defs

#define XML_BUFLEN 512
#define XML_DOCUMENT_TAG "\n"
#define XML_TREE_OPEN "<tree>\n"
#define XML_TREE_CLOSE "</tree>\n"
#define XML_NODE_OPEN "<node>"
#define XML_NODE_CLOSE "</node>\n"
#define XML_INFO_OPEN "<info>\n"
#define XML_INFO_CLOSE "</info>\n"
/*****************************************************************
data structures
 *****************************************************************/

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
  char * keychars;
  int length;
} bkey_t;

//occurrence counter, as a pointer to unsigned long
typedef unsigned long occurrence_count_t;

//single position
typedef unsigned long position_t;

//positions, as a linked list
typedef struct _ocp_{
  position_t position;
  struct _ocp_ * next;
} occurrence_position_t;


//an item collects a key, occurrence count and the head of the occurrence positions list
typedef struct {
  bkey_t * key;
  occurrence_count_t count;
  occurrence_position_t * occurs;
} item_t;


//a dictionary is a collection of items, implemented as a binary tree
typedef struct _node_{
  item_t * item;
  struct _node_ * father;
  struct _node_ * left;
  struct _node_ * right;
  long lefters;
  long righters;
} treenode_t;


/********************************************************
enums
 ********************************************************/
typedef enum {
  LINK_SAFE_MODE,  //return error if el->next is not null
  LINK_UNSAFE_MODE, //ignore value of el->next: will be overwritten
  LINK_UNSAFE_FREE_MODE, //free(el->next) before overwrite
  LINK_SEARCH_TAIL //find the tail element (el: el->next == 0), then attach
} link_policy_t;


/*********************************************************
functions
 *********************************************************/

/*****************************
general purpose functions
 *****************************/
void * xmalloc (size_t size);


/*****************************
key managing functions
 *****************************/

/********************************
add_key:
1) lookup dictionary for key
2.A) if not found (key)
        a) create new item (key, position, count = 0)
        b) create new node (item)
        c) return -1
2.B) else
        a) upcount node->item
        b) add position to occurrence_list
        c) set item = node->item
        c) return 0
 ********************************/
//add_key: Try and add or update  key in dictionary tree. 
item_t * add_key(treenode_t * dictionary, bkey_t * key, position_t position);
//find_key: lookup dictionary for key. if matched, set item pointer, return 0.
//else, set item = null, return -1 
item_t * find_key (treenode_t * dictionary, bkey_t * key);
bkey_t * create_key (char * buf, int len);
// returns 0 if equal; -1 if key1 is lesser than key2; 1 if key1 is greater than key 2.
int compare_keys (bkey_t * key1, bkey_t * key2); 


/********************************
item managing functions
***********************************/
item_t * create_item (bkey_t * key);
int set_count (item_t * item, occurrence_count_t count);
int add_occurrence (item_t * item, position_t pos);
int link_occurrence (occurrence_position_t * new
		     , occurrence_position_t * tail
		     , position_t pos
		     , link_policy_t policy);
int additem (treenode_t * dictionary, bkey_t * key);
occurrence_count_t upcount (item_t * item);




/********************************
dictionary tree managing functions
***********************************/

treenode_t * create_tree ();
int link_right (treenode_t * father, treenode_t * right);
int link_left (treenode_t * father, treenode_t * left);
int detach_right (treenode_t * father, treenode_t * right);
int detach_left (treenode_t * father, treenode_t * left);
int free_tree (treenode_t * root);
int prune_right (treenode_t * father);
int prune_left (treenode_t * father);
treenode_t * get_biggest (treenode_t * root);
treenode_t * get_smallest (treenode_t * root);
int get_xml (treenode_t * root);

/*****************
rotate: in this binary tree, when a subtree gets too big with respect to the other
, the tree needs to be rebalanced.
If the Left subtree becomes too big, the algorithm does the following:
1) determine N* = max (Left); the bigger node belonging to Left. This node has no "righters", by definition.
2) determine L = root(Left); the root of the left subtree
3) determine Root = the present root of the tree
4) determine R = root(Right); the root of the right subtree
5) determine pN* = parent(N*);
6) determine lN* = left(N*); the left child of N*, if it exists
7) Root --> R';
8) R --> right(R'); lefters(R') := 0; righters(R') := lefters(R) + righters(R);
9) N* --> Root'; righters(Root') := righters(R') + 1; lefters(Root') := lefters(L) + righters(L) - 1;
10) R' --> right(Root')
11) L --> left (Root') = L'
12) lN* --> right (pN*)
 *****************/
int rotate_left_right (treenode_t * root_node);
int rotate_right_left (treenode_t * root_node);

//check_balance: check tree balancing, then you'll decide wether to balance it or not
int check_balance (treenode_t * root_node, int threshold); 



