#include <stdio.h>
#include <semaphore.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

int main(){
    printf("Starting consumer...\n");

    // same setup as producer
    int file_descriptor = shm_open("/table", O_CREAT | O_RDWR, S_IRUSR);
    if(file_descriptor < 0) printf("Failed to create/access shared memory");

    ftruncate(file_descriptor, 8);

    int* ptr = mmap(NULL, 8, PROT_READ, MAP_SHARED, file_descriptor, 0);

    printf("The first value is: %d", *(ptr));
    printf("The second value is: %d", *(ptr+1));

    return 0;
}