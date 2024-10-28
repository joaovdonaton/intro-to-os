#include <stdlib.h>
#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define LOOP 3

int main(){
    printf("Starting consumer...\n");

    // same setup as producer
    sem_t *empty_sem = sem_open("/empty", O_CREAT, S_IRUSR | S_IWUSR, 1);
    if(empty_sem == SEM_FAILED){
        perror("Failed to open semaphore");
        exit(1);
    }

    sem_t *full_sem = sem_open("/full", O_CREAT, S_IRUSR | S_IWUSR, 0);
    if(full_sem == SEM_FAILED){
        perror("Failed to open semaphore");
        exit(1);
    }


    int file_descriptor = shm_open("/table", O_CREAT | O_RDWR, S_IRUSR);
    if(file_descriptor < 0) printf("Failed to create/access shared memory");

    ftruncate(file_descriptor, 8);


    int* ptr = mmap(NULL, 8, PROT_READ, MAP_SHARED, file_descriptor, 0);
    // consumer logic
    int i = 0;
    while(i < LOOP){
        sem_wait(full_sem);

        sleep(1); // simulate consuming

        printf("[Consumer] consumed %d from table\n", *ptr);
        printf("[Consumer] consumed %d from table\n", *(ptr+1));

        sem_post(empty_sem);
        i++;
    }

    return 0;
}