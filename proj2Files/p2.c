#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#define PAGESIZE getpagesize()
#define ALIGNMENT 16
// variables
void * region;
int * head;
int numByteCount;
bool wholeBlockalloc;
bool merged;
// data structures
typedef struct node {
  bool alloc;
  int size;
  char * this;
  struct node * next;
} node;

struct node * p;
struct node * newNode;
//will mask out last bit to zero ->>>>  int y = (x & -2);
void heap_init(int num_pages_for_heap)
{
    // allocate bytes requried by init.
    int numBytesToAllocate = (num_pages_for_heap * PAGESIZE);
    // set counter and allocate //
    numByteCount = numBytesToAllocate;
    region = mmap(NULL, (numBytesToAllocate), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    // set head pointer //
    head = region;
    // set flag //
    wholeBlockalloc, merged = false;
    //node newNode: //
    newNode = (node *) malloc(sizeof(node));
    newNode->size = numBytesToAllocate;
    newNode->this = region; //printf("&newNode->this: %p and &newNode: %p\n",&newNode->this, &newNode );
    newNode->next = NULL; //printf("&newNode->next: %p AND newNode->next: %p\n",&newNode->next, newNode->next );
    newNode->alloc = false; //printf("&newNode->alloc: %p AND newNode->alloc: %d\n",&newNode->alloc, newNode->alloc );
}

// returns a pointer to a chunck of data at least as big as asked for. alligned to a 16 byte boundary
// if num_bytes_to_allocate = zero return null/ if unsuccessful returns null and sets errno
void *heap_alloc(int num_bytes_to_allocate)
{
  // Reset p to the new node
  p = newNode;
  // reset alignment
  if(num_bytes_to_allocate % ALIGNMENT != 0){
    int bytes = num_bytes_to_allocate / 16 + 1;
    num_bytes_to_allocate = bytes * 16;
  }
  // check to see if block has been used up
  if(num_bytes_to_allocate > numByteCount){
      wholeBlockalloc == true;
    } else if(num_bytes_to_allocate < numByteCount) {
      wholeBlockalloc == false;
    }

  // if current block is not allocated and the the bloc still has bytes to give up //
  if( (p->alloc == false) && (numByteCount > num_bytes_to_allocate) ){
    numByteCount = (numByteCount - num_bytes_to_allocate);
    p->size = num_bytes_to_allocate;
    p->alloc = true;
    return p->this;
  } else {
    // traverse nodes
    while(p->next != NULL){
      p = (p->next);
      }
  if( (p->next == NULL) && (!merged) &&(wholeBlockalloc != true) && (numByteCount > num_bytes_to_allocate) ){
      node * finalNode = (node *) malloc(sizeof(node));
      numByteCount = (numByteCount - num_bytes_to_allocate);
      p->next = finalNode;
      finalNode->size = num_bytes_to_allocate;
      finalNode->this = (p->this + (num_bytes_to_allocate) );
      finalNode->alloc = true;
      finalNode->next = NULL;
      return finalNode->this;

    }
    if(merged == true) {
      p = newNode;
      while (p->next !=NULL) {
        if(p->alloc == false && p->size > num_bytes_to_allocate){
          p->size = num_bytes_to_allocate;
          p->alloc = true;
          return p->this;
        }
        p = p->next;
      }
    } else {
      return NULL;
    }

  }
//////////////////////////////////////////////////////////////////////////////////
}

// returns block to pool of free memory
void heap_free(void *pointer_to_area_to_free)
{
    p = newNode;
    while (p->next != NULL) {
      p = p->next;
      //printf("heap_free: p->next: %p\n", p->next );
      if(pointer_to_area_to_free == p->this){
        p->alloc = false;
      }
    }
    // coalesce adjacent unallocated blocks
    p = newNode;
    while (p->next !=NULL) {
      if(p->alloc == 0 && p->next->alloc == 0){
        merged = true;
        int newSize = ( (p->size) + (p->next->size) );
        p->size = newSize;
        numByteCount += newSize;
        p->next = p->next->next;
      }
      p = p->next;
    }
    return;
}
