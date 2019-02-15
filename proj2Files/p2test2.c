#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void heap_init(int num_pages_for_heap);
void *heap_alloc(int num_bytes_to_allocate);
void heap_free(void *pointer_to_area_to_free);

int main(int argc, char *argv[])
{
    char *p1, *p2, *p3, *p4, *p5, *p6;

    if (argc > 1  &&  strcmp(argv[1],"-hw") == 0)
    {
        printf("hello world\n");
        exit(0);
    }

    heap_init(1);
//////////////////////////////////////////////////
    p1 = (char *) heap_alloc(1000);
    if ((long int)p1 % 16 != 0)
    {
        printf("p1 bad %p  pmod16 %d\n",p1,((long int)p1)%16);
        exit(-1);
    }
    memset(p1,'A',1000);
    printf("p1 works: %p \n", p1 );
    printf("p1 works: %c \n\n", *(p1+999) );
///////////////////////////////////////////////////////
    p2 = (char *) heap_alloc(1000);
    if ((long int)p2 % 16 != 0)
    {
        printf("p2 bad %p  pmod16 %d\n",p2,((long int)p2)%16);
        exit(-1);
    }
    memset(p2,'B',1000);
    printf("p2 - p1 %d\n", (p2 - p1) );
    printf("p2 works: %p \n", p2 );
    printf("p2 works: %c \n\n", *(p2+999) );

    p3 = (char *) heap_alloc(1000);
    if ((long int)p3 % 16 != 0)
    {
        printf("p3 bad %p  pmod16 %d\n",p3,((long int)p3)%16);
        exit(-1);
    }
    memset(p3,'C',1000);
    printf("p3 - p2 %d\n", (p3 - p2) );
    printf("p3 works: %p \n", p3 );
    printf("p3 works: %c \n\n", *(p3+999) );

    p4 = (char *) heap_alloc(1000);
    if ((long int)p4 % 16 != 0)
    {
        printf("p4 bad %p  pmod16 %d\n",p4,((long int)p4)%16);
        exit(-1);
    }
    memset(p4,'D',1000);
    printf("p4 - p3 %d\n", (p4 - p3) );
    printf("p4 works: %p \n", p4 );
    printf("p4 works: %c \n\n", *(p4+999) );

    p5 = (char *) heap_alloc(1600);    // 1st try should fail
    if (p5 != NULL)
    {
        printf("p5 should have been NULL, but is %p\n",p5);
        exit(-1);
    }
    printf("pointer to p2: %p\n", (void *)(p2 ) );
    printf("p5 1st try works\n\n" );

    heap_free( (void *)p2 );
    p5 = (char *) heap_alloc(1600);    // 2nd try should fail
    if (p5 != NULL)
    {
        printf("p5 should have been NULL, but is %p\n",p5);
        exit(-1);
    }
    printf("p5 2nd try works\n" );

    heap_free( (void *)p3 );
    p5 = (char *) heap_alloc(1600);    // 3rd try should succeed
    if (p5 == NULL)
    {
        printf("p5 should NOT have been NULL here %p\n",p5);
        exit(-1);
    }
    if ((long int)p5 % 16 != 0)
    {
        printf("p4 bad %p  pmod16 %d\n",p4,((long int)p4)%16);
        exit(-1);
    }
    if ( *(p5+32) != 'B')     //  first few bytes should be B
    {
        printf("p5 (%p) first byte should be B but is %c\n",p5,*(p5+32));
        exit(-1);
    }
    if ( *(p5+1200) != 'C')  // bytes near the end should be C
    {
        printf("p5 (%p) end bytes should be C but are %c\n",p5,*(p5+1200));
        exit(-1);
    }

    printf("DONE\n");

    return 0;
}
