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
    wholeBlockalloc = false;
    //node newNode: //
    newNode = (node *) malloc(sizeof(node));
    printf("region this: %p and region end: %p\n", region, (region+numBytesToAllocate) );
    newNode->size = numBytesToAllocate;
    newNode->this = region; //printf("&newNode->this: %p and &newNode: %p\n",&newNode->this, &newNode );
    newNode->next = NULL; //printf("&newNode->next: %p AND newNode->next: %p\n",&newNode->next, newNode->next );
    newNode->alloc = false; //printf("&newNode->alloc: %p AND newNode->alloc: %d\n",&newNode->alloc, newNode->alloc );
    //p = newNode;
}

// returns a pointer to a chunck of data at least as big as asked for. alligned to a 16 byte boundary
// if num_bytes_to_allocate = zero return null/ if unsuccessful returns null and sets errno
void *heap_alloc(int num_bytes_to_allocate)
{
  //if(num_bytes_to_allocate % 16 != 0){num_bytes_to_allocate += num_bytes_to_allocate%16;}
  p = newNode;
  if(num_bytes_to_allocate % ALIGNMENT != 0){
    int bytes = num_bytes_to_allocate / 16 + 1;
    num_bytes_to_allocate = bytes * 16;
  }
  if(num_bytes_to_allocate > numByteCount) { wholeBlockalloc == true;}

  while(p->next != NULL){
    p = (p->next);
    printf("LOOP p &&&: %p & p->next: %p\n",p, p->next );
    }

  // if current block is not allocated and the the bloc still has bytes to give up //
  if( (p->alloc == false) && (numByteCount > num_bytes_to_allocate)){
    numByteCount = (numByteCount - num_bytes_to_allocate);
    printf("(1st if)numByteCount left after request: %d\n",numByteCount );
    p->size = num_bytes_to_allocate;
    p->alloc = true;
    printf("p->this: %p\n", p->this );
    return p->this;
  } else {
    // traverse nodes
  if( (p->next == NULL) && (wholeBlockalloc != true) && (numByteCount > num_bytes_to_allocate) ){
        node * finalNode = (node *) malloc(sizeof(node));
        numByteCount = (numByteCount - num_bytes_to_allocate);
        printf("(2nd if)numByteCount left after request: %d\n",numByteCount );
        p->next = finalNode;
        finalNode->size = num_bytes_to_allocate;
        finalNode->this = (p->this + (num_bytes_to_allocate) );
        finalNode->alloc = true;
        finalNode->next = NULL;
        free(p);
        return finalNode->this;

    } else {
      return NULL;
    }

  }
//////////////////////////////////////////////////////////////////////////////////
}

// returns bl;ock to pool of free memory
void heap_free(void *pointer_to_area_to_free)
{
    free(pointer_to_area_to_free);
    return;
}
