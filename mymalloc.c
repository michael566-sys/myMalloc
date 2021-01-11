#include "mymalloc.h"
#include <stdio.h>
#include <ctype.h>
#include <stddef.h>
#include <string.h>

#define STRUCT_SIZE (sizeof(Metadata))

// memory 
static char myblock[4096]; 

// mymalloc function allocated given bytes in memory, and get the file name and line where the myfree is called
void * mymalloc ( int bytes, char * file, int line  ) {
    
    //check if the input pointer is outof the scope of the memory
    if (DEBUG) {
        printf ("Entered Malloc Method\n");
        // printf ("address of start of memory: %d, end of memory: %d\n", ((int) &myblock[0])%100000, ((int) &myblock[4095]) % 100000 );
        // printf ( "location of myblock[0] is %d, location of myblock [4096]is %d\n", ((int) &myblock[0])%100000, ((int) &myblock[4096]) % 100000);
    }

    // check if requested size is 0 
    if ( bytes == 0 ) {
        // printf ("Input bytes is 0, no byte is allocated\n");
        printf ( "%s:%d: input bytes is 0, no byte is allocated: NULL pointer returned\n", file, line );

        return NULL; 
    }
    
    // variables for traversing the memory 
    Metadata * ptr = NULL; 
    Metadata * create_next = NULL; 
    ptr = (Metadata*) myblock;


    // if  myblock does not have metadata attached, means memory is empty 
    if ( ptr -> isFree == 0 && ( ptr -> blockSize == 0 ) ) {
        
        // check contents in the first block
        if (DEBUG){
            printf ( "DOES NOT HAVE FIRST BLOCK ALLOCATED !!!\n");
            printf ( "First block: isFree: %d, blockSize: %d \n", ((Metadata*)ptr)->isFree, ((Metadata*)ptr)->blockSize);
        }
        // initilize the very first metadata block in memory 
        ptr -> isFree = 1;
        ptr -> blockSize = sizeof (myblock) - STRUCT_SIZE;
        ptr -> next = NULL; 

    }   
    
    //if myblock meory already has a metadata attached, we then need to allocate memory 
    //Function for this while loop: traverse the whole memory to check for free space to allocate memory
    while ( ptr != NULL  ) {

        if (DEBUG){
            printf ( "FIRST BLOCK HAS ALREADY BEEN ALLOCATED !!!\n");
        }

        // if the blockSize is freed and the has exact the same space as bytes 
        if (  (ptr -> isFree == 1) && ( (ptr -> blockSize) == bytes )   ) {

            if (DEBUG){
                printf ("(ptr -> blockSize) == bytes ) \n");

              //  printf("curent address %d\n", (int) ptr );

            }

            // DEFREE current block and allocate bytes
            ptr -> isFree = 0 ; 
            ptr-> blockSize = bytes;

            if (DEBUG){

                printf ( "Memory allocated: isFree: %d, blockSize: %d \n", ptr->isFree, ptr->blockSize);
                printf("\033[0;32m");
             //   printf ( "%s:%d: success: at mymalloc method: allocate %d bytes: at address %d\n", file, line, bytes, ((int)(ptr + 1))%100000 );
                printf("\033[0m"); 
            }
            
            if (DEBUG) {
                printf ("At mymalloc.c: \n");
                check_memory();
            }

            // return the location of bytes instead of the location of metadata block
            return (ptr + 1) ;  


        } // if the block has more than space than bytes 
        else if ( (ptr -> isFree == 1) && ( (ptr -> blockSize) > bytes )) {
            
            if (DEBUG){
                printf ("( (ptr -> blockSize) > bytes ) \n");
            }

            // if blockSize has more space than sum of bytes and STRUCT_SIZE),
            // then we has enough space to split the current block into two blocks
            if  ( (ptr -> blockSize) >( bytes + STRUCT_SIZE ) ) {

                if (DEBUG){
                    printf ("(ptr -> blockSize) >( bytes + STRUCT_SIZE )  \n");
                }
            
                // split into two blocks 
                // The following is suitable for two scenarios:  
                // 1. if next block is not NULL 
                // 2. if next block is NULL 

                int temp_size = ptr -> blockSize; 
                
                Metadata * temp = ptr -> next; 

                if (DEBUG){
                  //  printf("curent address %d, isFree: %d, blockSize: %d \n", ((int)ptr)%100000, ptr -> isFree, ptr-> blockSize );
                }

                // DEFREE current block and allocate bytes
                ptr -> isFree = 0 ; 
                ptr -> blockSize = bytes;

                // allocate metadata for the second block
                create_next = (Metadata*) (((char*) ptr) + STRUCT_SIZE + bytes);
                
                if (DEBUG){
                     printf ("size of metadata: %lu, size of bytes: %d",STRUCT_SIZE , bytes);
                //     printf ("current create_next: %d ", ((int)ptr)%100000);
                //     printf("curent create_next: %d, isFree: %d, blockSize: %d \n", ((int)ptr)%100000, create_next -> isFree, create_next-> blockSize );
                 }

                 // DEFREE second block and assign values
                 create_next -> isFree = 1 ; 
                 create_next -> blockSize = temp_size - STRUCT_SIZE - bytes;
                 create_next -> next = temp; 

                if (DEBUG){
                 //   printf("next address %d\n", ((int) create_next)%100000 );
                }

                ptr -> next = create_next; 
          
                if (DEBUG) {
                 
                 
                 //   printf ( "Memory allocated: isFree: %d, blockSize: %d, at address: %d\n", ptr->isFree, ptr->blockSize, ((int)ptr)%100000 );
                    printf("\033[0;32m");
                  //  printf ( "%s:%d: success: at mymalloc method: allocate %d bytes: at address %d\n", file, line, bytes, ((int)(ptr+1))%100000);
                    printf("\033[0m"); 
                }
                if (DEBUG) {
                    printf ("At mymalloc.c: \n");
                check_memory();

                }
                // return the location of bytes instead of the location of metadata block

                return ( ptr + 1 ); 

            }
            // if blockSize <= (bytes +  STRUCT_SIZE), 
            // there is not enough space to split into two blocks 
            // so give the extra the memories together with bytes : at most give 16 bytes of extra memories 
            else {
                if (DEBUG){
                    printf ("( blockSize <= (bytes +  STRUCT_SIZE) ) \n");
                 //   printf("curent address %d, isFree: %d, blockSize: %d \n", ((int) ptr) % 100000, ptr -> isFree, ptr-> blockSize );
                }
                // DEFREE current block and allocate bytes
                ptr -> isFree = 0 ; 
                
                if (DEBUG){

                  //  printf ( "Memory allocated: isFree: %d, blockSize: %d, at address: %d\n", ptr->isFree, ptr->blockSize, ((int)ptr)%100000 );
                
                    // printf("\033[0;32m");

                    // printf ( "%s:%d: success: at mymalloc method: allocate %d bytes: at address %d \n", file, line, bytes, ((int)(ptr+1))%100000 );
                    // printf("\033[0m"); 
                }

                if (DEBUG) {
                    printf ("At mymalloc.c: \n");
                    check_memory();

                }

                return (ptr + 1) ; 

            }
                
                

            } 
            // not enough space for bytes
            else {
                // go to the next block
                ptr = ptr -> next; 
                continue; 
            }

    // end of while loop
    }

 
    // if a pointer has no been returned in this case, it means there is not enought memory
    // to allocate bytes, so memory saturation
    if ( ptr == NULL ) {
        printf ( "%s:%d: error: at mymalloc method: saturation of memory\n", file, line );
        return NULL; 
    }

    return NULL; 
    
    if (DEBUG){
        printf ("Exited Malloc Method\n\n");
    }
}


// free given pointer nameed, target_ptr, in memory, and get the file name and line where the myfree is called
void myfree ( void * target_ptr,  char * file, int line  ) {

    if (DEBUG){
        printf ("Entered Free Method\n");
        // printf ("address of start of memory: %d, end of memory: %d\n", ((int) &myblock[0])%100000, ((int) &myblock[4095]) % 100000 );
        // printf ( "location of myblock[0] is %d, location of myblock [4096]is %d\n", ((int) &myblock[0])%100000, ((int) &myblock[4096]) % 100000);
    }
    
    //check if the input pointer is outof the scope of the memory
    if ( (char*)target_ptr > &myblock[4095] || (char*)target_ptr < &myblock[0]  ) {
        printf ( "%s:%d: error: at myfree method: input pointer out of scope\n", file, line );
        return; 
    }

    // initalize pointers to traverse the myblock memory
    Metadata * ptr = NULL; 
    Metadata * compare = NULL; 
    ptr =  (Metadata*) myblock;


    //if the first metadata has not been create, then print error and stop the program 
    if ( ptr -> isFree == 0 && ( ptr -> blockSize == 0 ) ) {
        printf ( "%s:%d: error: at myfree method: no memory has been allocated\n", file, line );
        return; 
    }

    if (DEBUG){
        printf ( "Start block: isFree: %d, blockSize: %d \n", ptr ->isFree, ptr->blockSize);
    }
    // make sure compare points to the actual memory, not the start of Metadata
    compare = ptr + 1; 
    
    // scan through the array to search for the pointer and is equal to the target_pointer (use will loop)
    while ( ptr != NULL ){
        // if the target_ptr is not the same as the address at the start of the memory
        // then move pointers to the next memory block
        if ( target_ptr != (void*) compare ){

            if (DEBUG){
               // printf ("target_ptr: %d, compare: %d\n", ((int)target_ptr)%100000, ((int)compare)%100000);
            }

            ptr = ptr -> next ; 

            if ( ptr != NULL ) {
                if (DEBUG){

                    printf ( "Current block: isFree: %d, blockSize: %d \n", ptr->isFree, ptr->blockSize);
                }
                compare = ptr + 1; 
            }
            

        } 
        //if the target_ptr is the same as the address at the start of the memory
        // memory to be freed is found 
        else {
            
            // check pointer is freed or not
            // if it is already freed, print an error
            if ( ptr ->isFree == 1 ) {
                printf ( "%s:%d: error: at myfree method: pointer has already been freed\n", file, line );

                break; 

            }
            // if it is not freed, free it and print success message
            else {

                ptr -> isFree = 1; 
                if (DEBUG){
                    printf("\033[0;32m");
                   // printf ( "%s:%d : success: at myfree method: freed %d bytes: at address %d \n", file, line, ptr->blockSize, ((int)compare)%100000 );
                    printf("\033[0m"); 
                    printf ( "Freeing Success\n");
                }
                break ; 
            }
           
        }

    } 

    if ( ptr == NULL ) {
        printf ( "%s:%d : error: at myfree method: invalid pointer for freeing \n", file, line );
    }

    // call merge_freed_blocks function to merge freed memories that are closed to each other
    merge_freed_blocks (); 

    if (DEBUG){
        printf ("Exited Free Method\n\n");
    }

}

// this function merges and free blocks that are closed to each other my
// traversing the myblock memory using pointers, and uses pointers operation 
// to achieve memory merging 

void merge_freed_blocks () {

    Metadata * prev = NULL; 
    Metadata *  ptr =  (Metadata*) myblock; 
    
    prev = ptr; 
    ptr = ptr -> next; 

    // scan through the array to merge nearby freed block 
    while ( ptr != NULL ) {

        // use prev and ptr to check prev and ptr are both isFree == 1
        if (prev -> isFree == 1) {
            while ( ptr != NULL  ){
                if (ptr->isFree == 1) {
                    
                    // extend the blockSize of prev till the end of ptr and reallocate the next pointer
                    (prev -> blockSize) = (prev -> blockSize) + STRUCT_SIZE + (ptr -> blockSize); 
                   
                    if (DEBUG){
                        printf ("Current Prev blockSize: %d\n", prev -> blockSize) ;
                    }

                    (prev -> next) = (ptr -> next);                 
                    ptr = ptr -> next; 
                } else {
                    break;
                }
            }

        }

        prev = ptr; 
        if ( ptr != NULL ) {
            ptr = ptr -> next;
        }

    }

    if (DEBUG){
        printf ("check memory at merge_free_blocks\n");
        check_memory();
    }

}
/*
// traverse the memory and print out all information for each block
void check_memory() {

    Metadata * ptr = NULL; 
    Metadata * compare = NULL; 

    ptr =  (Metadata*) myblock; 

    if ( ptr -> isFree == 0 && ( ptr -> blockSize == 0 ) ) {
       
        printf ( "error: at myfree method: no memory has been allocated\n");
        
        return; 

    }

    printf ( "Checking memory: Start block: isFree: %d, blockSize: %d \n", ptr ->isFree, ptr->blockSize);
    compare = ptr + 1; 
    
    while ( ptr != NULL ){
            
        // printf ("target_ptr: %d, compare: %d\n", ((int)target_ptr)%100000, ((int)compare)%100000);
        ptr = ptr -> next ; 
        if ( ptr != NULL ) {
            printf ( "Checking memory: Current block: isFree: %d, blockSize: %d \n", ptr->isFree, ptr->blockSize);
            compare = ptr + 1; 
        }

    }

}
*/

// check whether the memory is empty or initalized with metadata
int check_empty () {

    Metadata * ptr = NULL; 
    ptr = (Metadata*) myblock;

    // if first block does not have metadata attached, means memory is empty 
    if ( ptr -> isFree == 0 && ( ptr -> blockSize == 0 ) ) {

        if (DEBUG){
        printf ( "DOES NOT HAVE FIRST BLOCK ALLOCATED !!!\n");
        printf ( "First block: isFree: %d, blockSize: %d \n", ((Metadata*)ptr)->isFree, ((Metadata*)ptr)->blockSize);
        }
        return 1; 

    }   

    // if first block does not has metadata attached, but the whole memory only has on freed block
    if (  ptr -> isFree == 1 && ( ptr -> blockSize == 4080 ) ) {
        if (DEBUG) {
            printf ( "DOES NOT HAVE FIRST BLOCK ALLOCATED !!! ONLY ONE BLOCK WITH ONE MEDTA DATA\n");
        }
        return 1; 
    }

    return 0 ;

}