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
    shm_unlink("/table"); 
    sem_unlink("/empty");
    sem_unlink("/full");

    printf("Starting producer...\n");

    // create semaphore with read/write permissions for owner
    sem_t *empty_sem = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if(empty_sem == SEM_FAILED){
        perror("Failed to open semaphore");
        exit(1);
    }

    // note that we initialize this to 0
    sem_t *full_sem = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(full_sem == SEM_FAILED){
        perror("Failed to open semaphore");
        exit(1);
    }

    // SHARED MEMORY

    // shared memory name, flag (create or read-write) and permissions mode
    // permissions here are set to WRITE for the owner of the process
    int file_descriptor = shm_open("/table", O_CREAT | O_RDWR, S_IRUSR | S_IWUSR);
    if(file_descriptor < 0) {
        printf("Failed to create/access shared memory");
        exit(1);
    }

    // establish size of object, we need 2 items (ints) so 8 bytes
    ftruncate(file_descriptor, 8);

    // so mmap maps the shared memory object to this processes address space, so it can accessed
    // we only give PROT_WRITE because this is the producer
    int* ptr = mmap(NULL, 8, PROT_READ | PROT_WRITE, MAP_SHARED, file_descriptor, 0);

    // PRODUCER LOGIC
    int i = 0;
    while(i < 10){   
        sem_wait(empty_sem);

        sleep(1); // simulate producing
        
        *ptr = rand()%100;
        printf("[Producer] wrote %d to table\n", *ptr);
        *(ptr+1) = rand()%100;
        printf("[Producer] wrote %d to table\n", *(ptr+1));
        
        sem_post(full_sem);
        i++;
    }

    // clean up 
    shm_unlink("/table"); 
    sem_unlink("/empty");
    sem_unlink("/full");

    return 0;
}