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

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "heaplist.h"

// HELPER FUNCTIONS
// swap two heap items (the items at index i and index j)
// assumes h != NULL
void swap_heaplist(Heaplist *h, int i, int j) {
	HeapItem t  = h->items[i];
	h->items[i] = h->items[j];
	h->items[j] = t;
}

// move an item up the heap structure to re-establish heap order
// bonus challenge: improve this function by avoiding some copies/swaps
// assumes h != NULL
void siftup_heaplist(Heaplist *h, int i) {
	int parent = (i - 1) / 2;
	while (h->items[i].key < h->items[parent].key) {
		swap_heaplist(h, parent, i);
		i = parent;
		parent = (i - 1) / 2;
	}
}

// find the index of the smallest child among the children of an item
// if the item has no children, return 0
// assumes h != NULL
int min_child_heaplist(Heaplist *h, int i) {
	int child = i * 2 + 1;
	if (child >= h->cur_size) {
		// no children
		return 0;
	} else if (child+1 >= h->cur_size || h->items[child].key < h->items[child+1].key) {
		// only child or first child is smallest child
		return child;
	} else {
		// second child exists and is smallest child
		return child+1;
	}
}

// move an item down the heap structure to re-establish heap order
// bonus challenge: improve this function by avoiding some copies/swaps
// assumes h != NULL
void siftdown_heaplist(Heaplist *h, int index) {
	int child = min_child_heaplist(h, index);
	while (child && h->items[child].key < h->items[index].key) {
		swap_heaplist(h, child, index);
		index = child;
		child = min_child_heaplist(h, index);
	}
}

// returns a pointer to a new, empty heap with a capacity for maximum_size items
Heaplist *new_heaplist(int max_size) {
	Heaplist *h = malloc(sizeof *h);
	assert(h);

	h->items = malloc(max_size * (sizeof *h->items));
	assert(h->items);

	h->cur_size = 0;
	h->max_size = max_size;

	return h;
}

// inserts data, pointer to the node in next doclist, into h with priority key
void heaplist_insert(Heaplist *h, int key, float data, Node *node, int index_term) {
	assert(h != NULL);
	assert(h->cur_size < h->max_size);

	h->items[h->cur_size].key  = key;
	h->items[h->cur_size].data = data;
	h->items[h->cur_size].node = node;
	h->items[h->cur_size].index_term = index_term;
	h->cur_size++;

	siftup_heaplist(h, h->cur_size-1);
}

// remove the item with the smallest key in h
float heaplist_remove_min(Heaplist *h) {
		assert(h != NULL);
		assert(h->cur_size > 0);

		float min = h->items[0].data;

		h->items[0] = h->items[h->cur_size-1];
		h->cur_size -= 1;
		siftdown_heaplist(h, 0);

		return min;
}

// return, but not remove, the item with the smallest key in h
float heaplist_peek_min(Heaplist *h) {
	assert(h != NULL);
	assert(h->cur_size > 0);

	return h->items[0].data;
}

// return the key of the item with the smallest key in h
int heaplist_peek_key(Heaplist *h) {
	assert(h != NULL);
	assert(h->cur_size > 0);

	return h->items[0].key;
}

// de-allocate all memory used by h
void free_heaplist(Heaplist *h) {
	assert(h != NULL);

	free(h->items);
	free(h);
}

// return, but not remove, the pointer with the smallest key in h
Node *heaplist_peek_node(Heaplist *h) {
	assert(h != NULL);
	assert(h->cur_size > 0);

	return h->items[0].node;
}
// return, but not remove, the index of the doclist with the smallest key in h
int heaplist_peek_index_term(Heaplist *h) {
	assert(h != NULL);
	assert(h->cur_size > 0);

	return h->items[0].index_term;
}


// TESTING FUNCTIONS

// print the contents of a heap to stdout
void print_heaplist(Heaplist *h) {
	assert(h != NULL);
	int i;

	printf("printing heap\n-------------\n");
	printf("heap array:\n");
	printf("   i: ");
	for (i = 0; i < h->cur_size; i++) {
		printf("%5d ", i);
	}
	printf("\n");
	printf("  id: ");
	for (i = 0; i < h->cur_size; i++) {
		printf("%5d ", h->items[i].key);
	}
	printf("\n");
	printf("score: ");
	for (i = 0; i < h->cur_size; i++) {
		printf("%5.1f ", h->items[i].data);
	}
	printf("\n");
}
