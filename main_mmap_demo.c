#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>

#define TOTAL_ORDER 20
#define NUM_PROCESS 3
#define SHARED_MEM_SIZE 4096

typedef struct {
    int orders[TOTAL_ORDER];
    int front;
    int rear;
    int stock;
    int completed;
    int total_revenue;
    pthread_mutex_t mutex_queue;
    pthread_mutex_t mutex_stock;
    pthread_mutexattr_t mutex_attr;
} SharedMemory;

void init_shared_memory(SharedMemory* shared) {
    shared->front = 0;
    shared->rear = 0;
    shared->stock = 100;
    shared->completed = 0;
    shared->total_revenue = 0;
    
    pthread_mutexattr_init(&shared->mutex_attr);
    pthread_mutexattr_setpshared(&shared->mutex_attr, PTHREAD_PROCESS_SHARED);
    
    pthread_mutex_init(&shared->mutex_queue, &shared->mutex_attr);
    pthread_mutex_init(&shared->mutex_stock, &shared->mutex_attr);
}

void enqueue(SharedMemory* shared, int order_id) {
    pthread_mutex_lock(&shared->mutex_queue);
    shared->orders[shared->rear] = order_id;
    shared->rear++;
    pthread_mutex_unlock(&shared->mutex_queue);
}

int dequeue(SharedMemory* shared) {
    pthread_mutex_lock(&shared->mutex_queue);
    if (shared->front >= shared->rear) {
        pthread_mutex_unlock(&shared->mutex_queue);
        return -1;
    }
    int order_id = shared->orders[shared->front];
    shared->front++;
    pthread_mutex_unlock(&shared->mutex_queue);
    return order_id;
}

void order_receiver_process(SharedMemory* shared, int process_id) {
    printf("[Process %d - OrderReceiver] Started\n", process_id);
    
    int orders_per_process = TOTAL_ORDER / NUM_PROCESS;
    int start = process_id * orders_per_process;
    int end = (process_id == NUM_PROCESS - 1) ? TOTAL_ORDER : start + orders_per_process;
    
    for (int i = start; i < end; i++) {
        enqueue(shared, i);
        printf("[Process %d] Order %d masuk ke shared queue\n", process_id, i);
        usleep(50000);
    }
    
    printf("[Process %d - OrderReceiver] Completed\n", process_id);
}

void kitchen_process(SharedMemory* shared, int process_id) {
    printf("[Process %d - Kitchen] Started\n", process_id);
    
    while (1) {
        int order_id = dequeue(shared);
        
        if (order_id == -1) {
            pthread_mutex_lock(&shared->mutex_stock);
            if (shared->completed >= TOTAL_ORDER) {
                pthread_mutex_unlock(&shared->mutex_stock);
                break;
            }
            pthread_mutex_unlock(&shared->mutex_stock);
            usleep(100000);
            continue;
        }
        
        printf("[Process %d - Kitchen] Memproses order %d\n", process_id, order_id);
        sleep(1);
        
        pthread_mutex_lock(&shared->mutex_stock);
        shared->stock--;
        shared->completed++;
        shared->total_revenue += 10000;
        int current_completed = shared->completed;
        int current_stock = shared->stock;
        int current_revenue = shared->total_revenue;
        pthread_mutex_unlock(&shared->mutex_stock);
        
        printf("[Process %d - Kitchen] Order %d selesai (Stock: %d, Completed: %d, Revenue: Rp %d)\n",
               process_id, order_id, current_stock, current_completed, current_revenue);
    }
    
    printf("[Process %d - Kitchen] Completed\n", process_id);
}

void demo_with_mmap() {
    printf("========================================\n");
    printf("  MEMORY MAPPING DEMO (mmap)\n");
    printf("  Multi-Process dengan Shared Memory\n");
    printf("========================================\n\n");
    
    printf("[Main] Allocating shared memory dengan mmap()...\n");
    SharedMemory* shared = mmap(NULL, sizeof(SharedMemory),
                                PROT_READ | PROT_WRITE,
                                MAP_SHARED | MAP_ANONYMOUS,
                                -1, 0);
    
    if (shared == MAP_FAILED) {
        perror("mmap failed");
        exit(1);
    }
    
    printf("[Main] Shared memory allocated at address: %p\n", (void*)shared);
    printf("[Main] Shared memory size: %zu bytes\n", sizeof(SharedMemory));
    printf("[Main] Initializing shared memory...\n\n");
    
    init_shared_memory(shared);
    
    printf("========================================\n");
    printf("  MEMORY LAYOUT\n");
    printf("========================================\n");
    printf("Shared Memory Address: %p\n", (void*)shared);
    printf("  - orders array:      %p (offset: %ld)\n", 
           (void*)shared->orders, (char*)shared->orders - (char*)shared);
    printf("  - front:             %p (offset: %ld)\n", 
           (void*)&shared->front, (char*)&shared->front - (char*)shared);
    printf("  - rear:              %p (offset: %ld)\n", 
           (void*)&shared->rear, (char*)&shared->rear - (char*)shared);
    printf("  - stock:             %p (offset: %ld)\n", 
           (void*)&shared->stock, (char*)&shared->stock - (char*)shared);
    printf("  - completed:         %p (offset: %ld)\n", 
           (void*)&shared->completed, (char*)&shared->completed - (char*)shared);
    printf("  - total_revenue:     %p (offset: %ld)\n", 
           (void*)&shared->total_revenue, (char*)&shared->total_revenue - (char*)shared);
    printf("  - mutex_queue:       %p (offset: %ld)\n", 
           (void*)&shared->mutex_queue, (char*)&shared->mutex_queue - (char*)shared);
    printf("  - mutex_stock:       %p (offset: %ld)\n", 
           (void*)&shared->mutex_stock, (char*)&shared->mutex_stock - (char*)shared);
    printf("========================================\n\n");
    
    printf("[Main] Creating %d child processes...\n\n", NUM_PROCESS + 2);
    
    pid_t pids[NUM_PROCESS + 2];
    
    for (int i = 0; i < NUM_PROCESS; i++) {
        pids[i] = fork();
        if (pids[i] == 0) {
            order_receiver_process(shared, i);
            exit(0);
        } else if (pids[i] < 0) {
            perror("fork failed");
            exit(1);
        }
    }
    
    sleep(1);
    
    for (int i = 0; i < 2; i++) {
        pids[NUM_PROCESS + i] = fork();
        if (pids[NUM_PROCESS + i] == 0) {
            kitchen_process(shared, NUM_PROCESS + i);
            exit(0);
        } else if (pids[NUM_PROCESS + i] < 0) {
            perror("fork failed");
            exit(1);
        }
    }
    
    printf("[Main] Waiting for all child processes to complete...\n\n");
    
    for (int i = 0; i < NUM_PROCESS + 2; i++) {
        int status;
        waitpid(pids[i], &status, 0);
        printf("[Main] Process %d (PID: %d) terminated\n", i, pids[i]);
    }
    
    printf("\n========================================\n");
    printf("         FINAL STATISTICS\n");
    printf("========================================\n");
    printf("Total Orders: %d\n", TOTAL_ORDER);
    printf("Completed: %d\n", shared->completed);
    printf("Final Stock: %d\n", shared->stock);
    printf("Total Revenue: Rp %d\n", shared->total_revenue);
    printf("Expected Stock: %d\n", 100 - TOTAL_ORDER);
    printf("Expected Revenue: Rp %d\n", TOTAL_ORDER * 10000);
    printf("========================================\n\n");
    
    if (shared->completed == TOTAL_ORDER && 
        shared->stock == (100 - TOTAL_ORDER) &&
        shared->total_revenue == (TOTAL_ORDER * 10000)) {
        printf("SUCCESS: Data consistency maintained!\n");
        printf("Memory mapping dengan mutex works correctly!\n");
    } else {
        printf("WARNING: Data inconsistency detected!\n");
    }
    
    printf("\n========================================\n");
    printf("  MEMORY MAPPING EXPLANATION\n");
    printf("========================================\n");
    printf("1. mmap() allocates shared memory region\n");
    printf("2. MAP_SHARED: Changes visible to all processes\n");
    printf("3. MAP_ANONYMOUS: No file backing (pure memory)\n");
    printf("4. PROT_READ | PROT_WRITE: Read/write permissions\n");
    printf("5. PTHREAD_PROCESS_SHARED: Mutex works across processes\n");
    printf("6. All child processes share same memory address\n");
    printf("7. Zero-copy IPC: Direct memory access\n");
    printf("8. Faster than pipe/socket for large data\n");
    printf("========================================\n\n");
    
    printf("[Main] Unmapping shared memory...\n");
    pthread_mutex_destroy(&shared->mutex_queue);
    pthread_mutex_destroy(&shared->mutex_stock);
    pthread_mutexattr_destroy(&shared->mutex_attr);
    
    if (munmap(shared, sizeof(SharedMemory)) == -1) {
        perror("munmap failed");
        exit(1);
    }
    
    printf("[Main] Shared memory unmapped successfully\n");
    printf("[Main] Demo completed!\n");
}

void demo_memory_mapped_file() {
    printf("\n========================================\n");
    printf("  MEMORY-MAPPED FILE DEMO\n");
    printf("  Persistent Storage dengan mmap()\n");
    printf("========================================\n\n");
    
    const char* filename = "order_log.dat";
    int fd = open(filename, O_RDWR | O_CREAT, 0666);
    if (fd == -1) {
        perror("open failed");
        return;
    }
    
    size_t file_size = 1024;
    if (ftruncate(fd, file_size) == -1) {
        perror("ftruncate failed");
        close(fd);
        return;
    }
    
    char* mapped = mmap(NULL, file_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (mapped == MAP_FAILED) {
        perror("mmap failed");
        close(fd);
        return;
    }
    
    printf("[File Mapping] File '%s' mapped to memory at %p\n", filename, (void*)mapped);
    printf("[File Mapping] Writing order logs to memory-mapped file...\n\n");
    
    sprintf(mapped, "FoodGo Order Log\n");
    sprintf(mapped + strlen(mapped), "================\n");
    sprintf(mapped + strlen(mapped), "Order 1: Nasi Goreng - Rp 25000\n");
    sprintf(mapped + strlen(mapped), "Order 2: Mie Ayam - Rp 15000\n");
    sprintf(mapped + strlen(mapped), "Order 3: Es Teh - Rp 5000\n");
    sprintf(mapped + strlen(mapped), "Total: Rp 45000\n");
    
    printf("Content written to memory (will be persisted to file):\n");
    printf("%s\n", mapped);
    
    if (msync(mapped, file_size, MS_SYNC) == -1) {
        perror("msync failed");
    } else {
        printf("[File Mapping] Memory synced to disk successfully\n");
    }
    
    munmap(mapped, file_size);
    close(fd);
    
    printf("[File Mapping] File unmapped\n");
    printf("[File Mapping] Check '%s' to verify persistence\n", filename);
    printf("========================================\n");
}

int main() {
    printf("\n");
    printf("====================================\n");
    printf("  FOODGO MEMORY MAPPING DEMO\n");
    printf("  Operating Systems Final Project\n");
    printf("====================================\n\n");
    
    demo_with_mmap();
    
    demo_memory_mapped_file();
    
    printf("\n====================================\n");
    printf("  KESIMPULAN\n");
    printf("====================================\n");
    printf("1. mmap() untuk shared memory antar process\n");
    printf("2. Lebih cepat dari IPC tradisional (pipe/socket)\n");
    printf("3. Zero-copy: Direct memory access\n");
    printf("4. Mutex PTHREAD_PROCESS_SHARED untuk synchronization\n");
    printf("5. Memory-mapped file untuk persistent storage\n");
    printf("6. Real use case: Multi-process FoodGo architecture\n");
    printf("====================================\n\n");
    
    return 0;
}
