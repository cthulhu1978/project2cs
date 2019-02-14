#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#define PAGESIZE getpagesize()
#define ALIGNMENT 16
#define GET(p) (*(unsigned int *)(p))
#define END -1
//#define struct node * node_ptr
void * region;
int * head;
static unsigned int numByteCount;
bool wholeBlockalloc;

typedef struct node {
  bool alloc;
  unsigned int size;
  struct node * this;
  struct node * next;
} node;
struct node * p;

//will mask out last bit to zero ->>>>  int y = (x & -2);
void heap_init(int num_pages_for_heap)
{
    // allocate bytes requried by init.
    int numBytesToAllocate = (num_pages_for_heap * PAGESIZE);
    // set counter //
    numByteCount = numBytesToAllocate;
    region = mmap(NULL, (numBytesToAllocate), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0 );
    // set head pointer //
    head = region;
    // set flag //
    wholeBlockalloc = false;
    node head;
    head.size = numBytesToAllocate;
    head.this = region;
    head.next = NULL;
    head.alloc = false;
    printf("head size %d\n",head.size );
    p = head.this;

}

// returns a pointer to a chunck of data at least as big as asked for. alligned to a 16 byte boundary
// if num_bytes_to_allocate = zero return null/ if unsuccessful returns null and sets errno
void *heap_alloc(int num_bytes_to_allocate)
{
  ////////////////////////////////////////////////////////////////////////////////////////
  // set flag if whole block is allocated //
  if(num_bytes_to_allocate > numByteCount){wholeBlockalloc == true;}
  // if current block is not allocated and the the bloc still has bytes to give up //
  if( (p->alloc == false) && (numByteCount > num_bytes_to_allocate)){
    // set size to num bytes needed and flip allocated bit, set next to null to tack on another node;
    p->size = num_bytes_to_allocate;
    p->alloc = true;
    p->next = NULL;
    // decrement
    numByteCount = (numByteCount - num_bytes_to_allocate);
  } else {
    // traverse nodes
    while(p->next != NULL){
      p = (p->next);
    }
    if(p->next == NULL && wholeBlockalloc != true){

    }
  }
//////////////////////////////////////////////////////////////////////////////////
}

// returns bl;ock to pool of free memory
void heap_free(void *pointer_to_area_to_free)
{
    return;
}
