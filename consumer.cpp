//g++ producer.cpp -pthread -lrt -o producer g++ consumer.cpp -pthread -lrt -o consumer
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

    sleep(1);

    int shm_fd = shm_open(SHM_NAME, O_RDWR, 0666);
    SharedData* data = (SharedData*)mmap(0, sizeof(SharedData),
                                         PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    sem_t* empty = sem_open(SEM_EMPTY, 0);
    sem_t* full  = sem_open(SEM_FULL, 0);
    sem_t* mutex = sem_open(SEM_MUTEX, 0);

    for (int i = 0; i < 10; i++) {
        sem_wait(full);
        sem_wait(mutex);

        int item = data->table[data->out];
        cout << "Consumed: " << item << endl;
        data->out = (data->out + 1) % TABLE_SIZE;

        sem_post(mutex);
        sem_post(empty);

        sleep(1);
    }

    munmap(data, sizeof(SharedData));
    close(shm_fd);
    return 0;
}