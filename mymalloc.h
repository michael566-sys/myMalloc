// macro for malloc
#define malloc( x ) mymalloc( x, __FILE__, __LINE__ )

// macro for myfree
#define free( x ) myfree( x, __FILE__, __LINE__ )

// on and off switch for comments that help debugging
#define DEBUG 0
#define DEBUG2 0 

// on and off switch for testcases for detecting errors
#define ERROR_CHECKING 0

// on and off switch for all workloads
#define WLA 1
#define WLB 1
#define WLC 1
#define WLD 1
#define WLE 1
#define WLF 1

// allocated given bytes in memory, and get the file name and line where the myfree is called
void * mymalloc ( int bytes ,  char * file, int line);

// free given pointer nameed, target_ptr, in memory, and get the file name and line where the myfree is called
void myfree ( void * target_ptr ,  char * file, int line );

// traverse the memory and print out all information for each block
//void check_memory();

// check whether the memory is empty or initalized with metadata
int check_empty();

// function to merge blocks that are already freed
void merge_freed_blocks (); 

// enum to label different workloads
enum { A, B, C, D, E, F};      

//struct for meta data
typedef struct node {

    int isFree;         // 4 bytes 
    int blockSize; // 4 bytes
    struct node *next; // 8 bytes

} __attribute__((packed)) Metadata ; // total 16 bytes





