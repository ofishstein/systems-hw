// For debugging in GDB with threads: methodology 1
//   (gdb) info threads
//   (gdb) thread 2
//   (gdb) # and so on for other threads
//   (gdb) where # each thread has its own stack
//   (gdb) frame 2  # to go to call frame 2

// For debugging in GDB: methodology 2
//   (gdb) break consumer
//   (gdb) run
//   (gdb) print buf
//   (gdb) next
//   (gdb) print buf  # and so on

#include <stdio.h>
#include <pthread.h> // Needed for pthread_mutex_lock(), etc.
#include <unistd.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
    int count;
    int pending_posts;
} sem_t;


// for semaphores
pthread_mutex_t mut = {0,0};
pthread_mutex_t mutp = {0,0};

// initializes a semaphore
void sem_init(sem_t *sem, int pshared, unsigned int value) {
    sem->count = value;
    sem->pending_posts = 0;
}

// increases the coun of the semaphore and updates pending_posts
void sem_post(sem_t *sem) {
    pthread_mutex_lock(&mut);
    sem->count++;
    if (sem->count <= 0) sem->pending_posts++;
    pthread_mutex_unlock(&mut);
}

// sem wait
void sem_wait(sem_t *sem) {
    pthread_mutex_lock(&mut);
    int done;
    sem->count--;
    if (sem->count < 0) {
        pthread_mutex_unlock(&mut);
    sleep:
        while (sem->pending_posts <= 0) {
            usleep(1);
        }
        pthread_mutex_lock(&mutp);
        if (sem->pending_posts > 0) {
            done = 1;
            sem->pending_posts--;
        } else {
            done = 0;
        }
        pthread_mutex_unlock(&mutp);
        if (!done) {
            goto sleep;
        }
        pthread_mutex_lock(&mut);
    }
    pthread_mutex_unlock(&mut);
}

#define LEN 4

typedef struct queue {
    int buf[LEN];
    int first_occupied_slot;
    int first_empty_slot;
    sem_t sem_producer;  
    sem_t sem_consumer;
    pthread_mutex_t mut_buf;
} queue;

queue* queues[2];

// create a queue
queue* make_queue()
{
    queue* que = malloc(sizeof(queue));
    que->first_occupied_slot = 0;
    que->first_empty_slot = 0;
    sem_init(&que->sem_producer, 0, LEN);
    sem_init(&que->sem_consumer, 0, 0);
    //que->mut_buf = {0,0};
    return que;
}

// add item to the buffer
void push_buf(int val, queue* que) {
    que->buf[que->first_empty_slot] = val;
    que->first_empty_slot++;
    if (que->first_empty_slot >= LEN)
        que->first_empty_slot = 0;
}


// remove item from the buffer
int take_from_buf(queue* que) {
    int val = que->buf[que->first_occupied_slot];
    que->first_occupied_slot++;
    if (que->first_occupied_slot >= LEN)
        que->first_occupied_slot = 0;
    return val;
}


// given in assignment
void *producer(void *arg) {
  int work_item = 1;
  while (1) {
    int queue_num = rand() % 2;
    queue* que = queues[queue_num];
    usleep( rand() % 5 );
    sem_wait(&que->sem_producer);  // Wait for empty slots
    pthread_mutex_lock(&que->mut_buf);
      push_buf(work_item++, que);  // inside critical section with mut_buf lock
    pthread_mutex_unlock(&que->mut_buf);
    sem_post(&que->sem_consumer);  // Tell the consumer there's a new work item
  }
}

// given in assignment 
// Exactly the same, but the inverse:
void *consumer(void *arg) {
  while (1) {
    int work_item;
    int queue_num = rand() % 2;
    queue* que = queues[queue_num];
    usleep( rand() % 5 );
    sem_wait(&que->sem_consumer);
    pthread_mutex_lock(&que->mut_buf);
      work_item = take_from_buf(que);
    pthread_mutex_unlock(&que->mut_buf);
    sem_post(&que->sem_producer);

    printf("%d ", work_item);
    fflush(stdout);  // Force printing now; don't wait for the newline
  }
}

void run_threads(int count) {
        
    // create producer and consumer threads
    while (count > 0) {
        count--;
        
        pthread_t pp; // producer thread
        pthread_t cc; // consumer thread
        
        int perr = pthread_create(&pp, NULL, &producer, NULL);
        if (perr != 0) {
            printf("Failed to create producer thread\n");
        }
        
        int cerr = pthread_create(&cc, NULL, &consumer, NULL);
        if (cerr != 0) {
            printf("Failed to create consumer thread\n");
        }
    }
    
    // lock main thread to keep it from exiting 
    while (1) usleep(1);
}

int main() {
  // ... uses pthread_create to start producer and consumer
  // WARNING:  the primary thread runs main().  When main exits, the primary
  //             thread exits

  // for functionality for part 2 use run_threads(1)
  // for functionality for part 3 use run_threads(3)

  queues[0] = make_queue();
  queues[1] = make_queue();

  run_threads(3);

  return 0;
}
