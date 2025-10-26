#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
using namespace std;

#define TABLE_SIZE 2
#define SHM_NAME "/table_shm"
#define SEM_EMPTY "/sem_empty"
#define SEM_FULL  "/sem_full"
#define SEM_MUTEX "/sem_mutex"

struct SharedData {
    int table[TABLE_SIZE];
    int in;
    int out;
};

int main() {
    shm_unlink(SHM_NAME);  
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);

    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, sizeof(SharedData));
    SharedData* data = (SharedData*)mmap(0, sizeof(SharedData),
                                         PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    data->in = 0;
    data->out = 0;

    sem_t* empty = sem_open(SEM_EMPTY, O_CREAT, 0666, TABLE_SIZE);
    sem_t* full  = sem_open(SEM_FULL,  O_CREAT, 0666, 0);
    sem_t* mutex = sem_open(SEM_MUTEX, O_CREAT, 0666, 1);

    for (int item = 1; item <= 10; item++) {
        sem_wait(empty);
        sem_wait(mutex);

        data->table[data->in] = item;
        cout << "Produced: " << item << endl;
        data->in = (data->in + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(full);

        sleep(1);
    }

    munmap(data, sizeof(SharedData));
    close(shm_fd);

    shm_unlink(SHM_NAME);
    sem_unlink(SEM_EMPTY);
    sem_unlink(SEM_FULL);
    sem_unlink(SEM_MUTEX);
    return 0;
}