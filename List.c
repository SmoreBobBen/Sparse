//-----------------------------------------------------------------------------
// List.c
// programmer: Benjamin Watkins
// cruzid: bdwatkin
// assignment: pa4 (mostly copied from pa2 version)
//
// based on code from https://people.ucsc.edu/~ptantalo/cse101/Fall24/pa1.pdf
//-----------------------------------------------------------------------------

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

// Bones -----------------------------------------------------------------------
typedef struct NodeObj *Node; // definition to  be used in NodeObj type

typedef struct NodeObj { // private type (not in .h) for linked list nodes
  void *data;
  Node previous;
  Node next;
} NodeObj;

typedef struct ListObj { // private/non-exported
  Node front;
  Node back;
  Node cursor;
  int length;     // list length
  int curseIndex; // index of cursor, set to -1 if undefined
} ListObj;

typedef struct ListObj
    *List; // exported version of List, to be used in other code

// Node functions --------------------------------------------------------------
// Node Constructors-Destructors -----------------------------------------------
Node newNode(void *x) {
  Node newNode = calloc(1, sizeof(struct NodeObj));
  newNode->data = x;
  newNode->next = NULL;
  newNode->previous = NULL;
  return newNode;
}

void freeNode(Node *pN) {
  if (pN != NULL && *pN != NULL) {
    // printf("freeing %i\n", (*pN)->data);
    free(*pN);
    pN = NULL;
    // printf("freed\n");
  }
}

// Node Navigation Functions
Node *nextNode(Node *n) { // get pointer to next node from pointer
  Node next = (*n)->next;
  Node *pNext = &next;
  return pNext;
}

// freeChain
void freeChain(Node *pN, int x) { // frees (x) number of connected nodes.
                                  // the idea is to take the length of a list
                                  // and its first node and free all of the
                                  // nodes in that list.
  if (pN == NULL) {
    return;
  }
  // printf("entering freeChain for Node %i, chain length %i\n", (*pN)->data,
  // x);
  if (x <= 1) {
    if (pN != NULL) {
      freeNode(pN);
    }
  } else {
    // Node* pNext = nextNode(pN);
    freeChain(&((*pN)->next), (x - 1));
    freeNode(pN);
  }
}

// Constructors-Destructors ---------------------------------------------------
// moved to the end of the file to allow use of above defined functions like
// length()
List newList(void) {
  List newList = calloc(1, sizeof(struct ListObj));
  newList->length = 0;
  newList->curseIndex = -1;
  return newList;
}

void freeList(List *pL) {
  if (pL != NULL) {
    // get a pointer to the first node, and pass to freeChain
    if ((*pL)->front != NULL) {
      Node first = (*pL)->front;
      Node *pFirst = &first;
      freeChain(pFirst, (*pL)->length);
    }

    // free the list itself
    free(*pL);
    pL = NULL;
  }
}

// Access functions -----------------------------------------------------------
int length(List L) { return L->length; }
int index(List L) {
  return L->curseIndex; // make sure to always set index to -1 if undefined
}
void *front(List L) { return L->front->data; }
void *back(List L) { return L->back->data; }
void *get(List L) {
  /*int index = 0;		//Iterate this index dummy until it matches
  curseIndex Node current = L->front;//Current node being navigated, might need
  to change
                          // this to L->back, depending on definition
  while (index < L->curseIndex) {
          current = current->next;
          index++;
  }
  return current->data;*/
  if (L->cursor != NULL) {
    return L->cursor->data;
  } else {
    // fprintf(stderr, "get: called on undefined cursor\n");
    return NULL;
  }
}

// Manipulation procedures ----------------------------------------------------
void set(List L, void *x) { // Overwrites the cursor element’s data with x.
                            // Pre: length()>0, index()>=0
  if (L->length > 0 && L->curseIndex >= 0) {
    L->cursor->data = x;
  }
}

void moveFront(List L) { // If L is non-empty, sets cursor under the front
                         // element, otherwise does nothing.
  if (L->length != 0) {
    L->curseIndex = 0;
    L->cursor = L->front;
  }
}

void moveBack(List L) { // If L is non-empty, sets cursor under the back
                        // element, otherwise does nothing.
  if (L->length != 0) {
    L->curseIndex = (L->length) - 1;
    L->cursor = L->back;
  }
}

void movePrev(List L) { // If cursor is defined and not at front, move cursor
                        // one step toward the front of L; if cursor is defined
                        // and at front, cursor becomes undefined; if cursor is
                        // undefined do nothing
  if (L->curseIndex == 0) { // defined and at front
    L->curseIndex = -1;     // cursor becomes undefined
    L->cursor = NULL;
  } else if (L->curseIndex > 0) { // index not negative or 0
    L->curseIndex--;
    L->cursor = L->cursor->previous;
  }
}

void moveNext(List L) { // If cursor is defined and not at back, move cursor one
                        // step toward the back of L; if cursor is defined and
                        // at back, cursor becomes undefined; if cursor is
                        // undefined do nothing
  if (L->curseIndex == (L->length - 1)) { // If cursor is at the back
    L->curseIndex = -1;                   // becomes undefined
    L->cursor = NULL;
  } else if (L->curseIndex >= 0) { // If cursor is defined and not at back
    L->curseIndex++;
    L->cursor = L->cursor->next;
  }
}

void prepend(List L, void *x) {
  // Insert new element into L. If L is non-empty, insertion takes place before
  // front element. List consists of front node (F) and back node (B) and many
  // nodes in-between [ (F) -> (...) -> (B) ]L
  // printf("prepending %i\n", x);
  Node pp = newNode(x); // create new node using integer input (N)

  // linking
  pp->next = L->front;  // (N) -> [(F) -> (...) -> (B)]L
  L->front = pp;        // [(N) -> (F) -> (...) -> (B)]L
  if (L->length == 0) { // if list is empty before prepend, pp is also the back
    L->back = pp;
  } else if (L->length > 0) {
    pp->next->previous = pp;
  }
  // deal with cursor
  if (L->curseIndex != -1) {
    L->curseIndex++;
  }

  // increment length
  L->length++;
}

void append(List L, void *x) {
  // Insert new element into L. If L is non-empty, insertion takes place after
  // back element. prepend but backwards
  // printf("appending %i\n", x);
  Node ap = newNode(x); // create new node using integer input (N)

  // doubly link nodes
  ap->previous = L->back;
  L->back = ap;
  if (L->length == 0) {
    L->front = ap;
  } else {
    ap->previous->next = ap;
  }
  L->length++;
}

void insertBefore(List L, void *x) { // Insert new element before cursor.
  if (L->length > 0 && L->curseIndex >= 0) { // Pre: length()>0, index()>=0
    Node insert = newNode(x);                // create new node

    // link new node to pre-existing ones
    insert->next = L->cursor;
    if (L->cursor->previous != NULL) {
      insert->previous = L->cursor->previous;
      insert->previous->next = insert;
    }
    // link pre-existing nodes to new one
    L->cursor->previous = insert;
    if (L->curseIndex == 0) { // cursor is at front, so the insertion
      L->front = insert;      // is the new front.
    }
    L->curseIndex++;
    L->length++;
  }
}

void insertAfter(List L, void *x) {          // Insert new element after cursor.
  if (L->length > 0 && L->curseIndex >= 0) { // Pre: length()>0, index()>=0
    Node insert = newNode(x);                // create new node

    // link new node to pre-existing ones
    insert->previous = L->cursor;
    insert->next = L->cursor->next;
    // link pre-existing nodes to new one
    L->cursor->next = insert;
    if (insert->next != NULL) { // check that there is a next element
      insert->next->previous = insert;
    }
    if (L->curseIndex ==
        (L->length - 1)) { // cursor is at back, so the insertion
      L->back = insert;    // is the new back.
    }
    L->length++;
  }
}

void deleteFront(List L) { // Delete the front element. Pre: length()>0
  if (L->length > 0) {
    // printf("deleting %i\n", L->front->data);
    Node tempNode = L->front;
    if (L->length > 1) {
      L->front->next->previous = NULL;
    }
    L->front = L->front->next;
    if (L->curseIndex == 0) {
      L->curseIndex = -1;
      L->cursor = NULL;
    } else if (L->curseIndex > 0) {
      L->curseIndex--;
    }
    freeNode(&tempNode);
    L->length--;
  }
}

void deleteBack(List L) { // Delete the back element. Pre: length()>0
  // using this for clear creates errors in valgrind, not sure why. No
  // memory leaks, just bad free errors?
  if (L->length > 0) {
    // printf("deleting %i\n", L->back->data);
    Node tempNode = L->back;
    if (L->length > 1) {
      L->back->previous->next = NULL;
    }
    L->back = L->back->previous;
    if (L->curseIndex == (L->length - 1)) {
      L->curseIndex = -1;
      L->cursor = NULL;
    }
    if (tempNode != NULL) {
      freeNode(&tempNode);
    }
    L->length--;
  }
}

void delete (List L) { // Delete cursor element, making cursor undefined.
                       // Pre: length()>0, index()>=0
  if (L->length != 0 && L->curseIndex >= 0) {
    // printf("deleting %i\n", L->cursor->data);
    Node tempNode = L->cursor;
    // stitch up hole
    if (L->curseIndex == 0) {
      L->front = tempNode->next;
    }
    if (L->curseIndex == (L->length - 1)) {
      L->back = tempNode->previous;
    }
    if (tempNode->next != NULL) {
      tempNode->next->previous = tempNode->previous;
    }
    if (tempNode->previous != NULL) {
      tempNode->previous->next = tempNode->next;
    }

    L->curseIndex = -1;
    L->cursor = NULL;
    freeNode(&tempNode);
    L->length--;
  }
}

void clear(List L) { // moved to end to allow use of deleteFront()
  // iterate through all members of the list and free them
  // or just implement deleteBack or deleteFront lol
  while (L->length > 0) {
    deleteFront(L);
  }
}

// Other operations -----------------------------------------------------------
void printList(FILE *out, List L) {
  // iterate through list L and print each node followed by a space
  Node current = L->front;
  // iterate a number of times equal to the length, printing each number
  // followed by a space, except for the last number
  for (int i = 1; i <= L->length; i++) {
    fprintf(out, "placeholder"); // TODO make work for void*
    current = current->next;
    if (i < L->length) {
      fprintf(out, " ");
    }
  }
  // fprintf(out, "\n");
}
