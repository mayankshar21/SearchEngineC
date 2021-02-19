/* * * * * * *
 * Module for creating and manipulating binary min-heaps on integer indices
 *
 * created for COMP20007 Design of Algorithms 2013
 * by Andrew Turpin
 * modified by Matt Farrugia <matt.farrugia@unimelb.edu.au>
 *
 * Further modifications made by Mayank Sharma <mayanks1@student.unimelb.edu.au>
 * this heap is call heaplist as it stores the pointers of the node in the doclist
 * along with the score, id and index of the doclist
 * extra datatypes like a node variable and the doclist index index_term
 * also the insert function has been updated to keep track of the node and doclist index as well
 * hence peek function for doclist and index have also been created
 */
 
#include "list.h"

 typedef struct item {
 	int key;  // the key for deciding position in heap
 	float  data;  // the data value associated with this key
  int index_term;
  Node *node;
 } HeapItem;

typedef struct heaplist Heaplist;

 struct heaplist {
 	HeapItem *items;  // the underlying array
 	int cur_size; // the number of items currently in the heap
 	int max_size; // the maximum number of items allowed in the heap
 };


// returns a pointer to a new, empty heap with a capacity for max_size items
Heaplist *new_heaplist(int max_size);

// inserts data, pointer to the node in next doclist, into h with priority key
void heaplist_insert(Heaplist *h, int key, float data, Node *node, int index_term);

// remove and return the item with the smallest key in h
float heaplist_remove_min(Heaplist *h);

// remove, but not remove, the pointer with the smallest key in h
float heaplist_peek_min(Heaplist *h);

// return the key of the item with the smallest key in h
int heaplist_peek_key(Heaplist *h);

// return, but not remove, the pointer with the smallest key in h
Node *heaplist_peek_node(Heaplist *h);

// return, but not remove, the index of the doclist with the smallest key in h
int heaplist_peek_index_term(Heaplist *h);

// de-allocate all memory used by h
void free_heaplist(Heaplist *h);


// FOR TESTING

// print the contents of a heap to stdout
void print_heaplist(Heaplist *h);
