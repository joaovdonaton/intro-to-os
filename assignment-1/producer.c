#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include <time.h>
#include <stdlib.h>

int main(){
    srand(time(NULL)); // seed for rng

    printf("Starting producer...\n");

    // create semaphore with read/write permissions for owner
    sem_unlink("/semaphore");
    sem_t *sem = sem_open("/semaphore", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if(sem == SEM_FAILED){
        perror("Failed to open semaphore");
        exit(1);
    }

    // SHARED MEMORY
 
    shm_unlink("/table");

    // shared memory name, flag (create or read-write) and permissions mode
    // permissions here are set to WRITE for the owner of the process
    int file_descriptor = shm_open("/table", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(file_descriptor < 0) {
        printf("Failed to create/access shared memory");
        exit(1);
    }

    // establish size of object, we need 2 items (in this case I'll use int) so 8 bytes
    ftruncate(file_descriptor, 8);

    // so mmap maps the shared memory object to this processes address space, so it can accessed
    // we only give PROT_WRITE because this is the producer
    int* ptr = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);


    // PRODUCER LOGIC

    sem_wait(sem);

    *ptr = rand()%100;
    *(ptr+1) = rand()%100;

    sem_post(sem);

    return 0;
}