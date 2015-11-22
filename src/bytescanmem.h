/*******************************
bytescanmem.h
Manage data structures for bytescan stats
 ******************************/
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


//an item collects a key, occurrence count and the head of the occurrence positions list
typedef struct {
  key_t * key;
  occurrence_count_t * count;
  occurrence_position_t * occurs;
} item_t;


//a dictionary is a collection of items, implemented as a binary tree
typedef struct {
  item_t * item;
  treenode_t * father;
  treenode_t * left;
  treenode_t * right;
  long lefters;
  long righters;
} treenode_t;

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

key_t * create_key (char ** buf, int len);
// returns 0 if equal; -1 if key1 is lesser than key2; 1 if key1 is greater than key 2.
int compare_keys (key_t * key1, key_t * key2); 


/********************************
item managing functions
***********************************/
int additem (treenode_t * dictionary, key_t * key);
int upcount (key_t * key);




/********************************
dictionary tree managing functions
***********************************/

int create_tree (treenode_t * root_node);
int link_right (treenode_t * father, treenode_t * right);
int link_left (treenode_t * father, treenode_t * left);
int get_father (treenode_t * father, treenode_t * child);
int get_left (treenode_t * left, treenode_t * father);
int get_right (treenode_t * right, treenode_t * father);
int get_biggest (treenode_t * biggest, treenode_t * root);
int get_smallest (treenode_t * smallest, treenode_t * root);

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
int check_balance (treenode_t * root_node); 



