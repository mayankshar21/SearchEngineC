/* ... */
#include<stdlib.h>
#include <stdio.h>
#include "query.h"
#include "list.h"
#include "heap.h"
#include "index.h"
#include "heaplist.h"

void print_array_results(Index *index, int n_results, int n_documents) {
	// check if n_results is not zero
	if(n_results == 0) {
		exit(0); // exit program
	}

	float doc_score[n_documents];         // initialise an array of size n_documents
	for(int i = 0;i < n_documents;i++) {
		doc_score[i] = 0.0;               // set all array index to zero
	}

	// code derived from index.c file
	// traverse through each list in the index
	// add the score to array with index as document id
	for(int i = 0;i<index->num_terms;i++) {
			for(Node *node = index->doclists[i]->head; node ;node = node->next) {
				Document *d = node->data;
				doc_score[d->id] += d->score;
			}
	}
	// initialise min heap with n_results
	Heap *h = new_heap(n_results);

	// loop to perform top k selection algorithm
	for(int i = 0;i < n_documents;i++) {
			if(i < n_results) {
				heap_insert(h, doc_score[i], i);   // if less than n_result add to heap
			} else {
			if(doc_score[i]> heap_peek_key(h)) { // else compare array with heap
				heap_remove_min(h);                // remove if heap score smaller
				heap_insert(h, doc_score[i], i);   // and insert array score into heap
			}
		}
	}
	print_heap(h); // print the contents of the heap
	free_heap(h); // free heap
}

void print_merge_results(Index *index, int n_results) {
	// check if n_results is not zero
	if(n_results == 0) {
		exit(0);
	}
	// initialise heap called heaplist to order the document lists with size query terms
	Heaplist *hl = new_heaplist(index->num_terms);
	Node *temp1;             // initialise 1st temporary node for traversal
	List *l = new_list();    // initialise a new list
	int n_size = 1;          // initialise n_size to 1
	void *index_array[index->num_terms];  // initial array containing the head of all doclist

	// for loop to add document id, score, node of each doclist and doclist number into heaplist
	for(int i = 0;i<index->num_terms;i++) {

		Node *temp = index->doclists[i]->head;
	  index_array[i] = temp;
		temp1 = index_array[i];
		Document *d = temp1->data;

		heaplist_insert(hl, d->id, d->score, temp1, i);
	}

	// for loop to perform the merge algorithm
	for(;hl->cur_size > 0;) {
		Node *temp3 = heaplist_peek_node(hl);  // peek node of document with min id
		int index_term = heaplist_peek_index_term(hl);  // peek index_term of document with min id
		heaplist_remove_min(hl);  // remove min id from heap
		Document *d = temp3->data;

		// code to keep track of size for array initialisation
		if(n_size < d->id) {
			n_size = d->id;
		}
		list_add_end(l, temp3->data);     // add node of the min id to list

		index_array[index_term] = temp3->next;  // updating array contain pointer to each doclist
		if(temp3->next != NULL) {
			Document *d1 = temp3->next->data;
			heaplist_insert(hl, d1->id, d1->score, temp3->next, index_term);  // inserting next document from the doclist into heap
		}
	}

	float new_doc_score[n_size];  // initialise array of size n_size with to add score
	for(int i = 0; i<n_size; i++) {
		new_doc_score[i] = 0;       // set all index to zero
	}

	// for loop to traverse through the list and and add the score of document id
	for(Node* node = l->head; node; node = node->next) {
		Document *d = node->data;
		new_doc_score[d->id] += d->score;
	}

	// initialise min heap with n_results
	Heap *h = new_heap(n_results);

	// loop to perform top k selection algorithm
	for(int i = 0;i < n_size;i++) {
		if(i < n_results) {
			heap_insert(h, new_doc_score[i], i);        // if less than n_results add to heap
		} else {
				if(new_doc_score[i]> heap_peek_key(h)) {  // else compare array with heap
					heap_remove_min(h);											// remove if heap score smaller
					heap_insert(h, new_doc_score[i], i);    // and insert array score into heap
				}
			}
	}
	print_heap(h);    // print the contents of the heap
	free_heap(h);     // free heap
	free_heaplist(hl);  // free heaplist
	free_list(l);       // free list
}
