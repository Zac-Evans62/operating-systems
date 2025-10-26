Compilation: 
g++ producer.cpp -pthread -lrt -o producer
g++ consumer.cpp -pthread -lrt -o consumer

run:
./producer & ./consumer &

Program Description:
This program impliments a version of the producer consumer problem using shared memory and semaphores. The producer generates a sequence of 10 items that are placed in a bounded buffer located in shared memory. Simultaniousely the consumer takes items from the same buffer. The buffer has a max capacity of two items. When the buffer is full the producer waits. When the buffer is empty the consumer waits. Too ensure the two processes have proper communication and to ensure mutual exclusion the processes use POSIX semaphores to manage access to shared recourses.

Key components: 
Semaphores: 3 semaphores were used in this program m
mutex - a semaphore that enforced mutual exclusion
enpty - reprisented the number of empty slots in the buffer
full - reprisented the number of filled slots in the buffer
All together these sempahores ensured only one process could access shared memory at a time, the producer waits while the buffer is full, and the consumer waits when its empty

Shared Memory:
The shared memory segment acted as a small buffere that could hold a max of two items. $ main commands were used to ensure that shared memory was created and used properly
shmget() - created the shared mamory
shmat() - maps the shared memory to each processes address space
shmdt() - detaches the shared memory from each process
shmctl() - frees system resources

Mutual exclusion:
initially the semaphore mutex was initilized to 1 meaning the shared region is available
sem_wait(mutex) effectively locks the critical section and is called before a process would modify the shared buffer.
once the operation is complete sem_post(mutex) is called to effectively unlock the shared memory allowing the other process to modify it.

proper output:

