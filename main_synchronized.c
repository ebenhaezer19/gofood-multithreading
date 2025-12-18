#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define TOTAL_ORDER 30
#define KITCHEN_COUNT 5

// Scheduling Metrics Structure
typedef struct {
    int order_id;
    int burst_time;        // Waktu memasak (detik)
    int arrival_time;      // Waktu order masuk (detik)
    int start_time;        // Waktu mulai diproses
    int completion_time;   // Waktu selesai
    int waiting_time;      // Waktu tunggu
    int turnaround_time;   // Total waktu (arrival -> completion)
    int worker_id;         // Kitchen worker yang handle
} OrderMetrics;

int order_queue[100];
int front = 0, rear = 0;

int payment_status[TOTAL_ORDER];
int stock = 100; // contoh stok bahan
int total_revenue = 0; // total pendapatan

// Scheduling metrics
OrderMetrics order_metrics[TOTAL_ORDER];
int burst_times[TOTAL_ORDER];  // Pre-generated burst times
time_t start_time_global;      // Program start time

pthread_mutex_t mutex_queue;
pthread_mutex_t mutex_stock;
pthread_mutex_t mutex_payment;
pthread_mutex_t mutex_metrics;  // Protect scheduling metrics

pthread_cond_t cond_payment;
pthread_cond_t cond_done;

int completed = 0;

// ------------------------------
// Queue functions
// ------------------------------
void enqueue(int orderID) {
    pthread_mutex_lock(&mutex_queue);  // LOCK queue
    order_queue[rear++] = orderID;
    pthread_mutex_unlock(&mutex_queue);
}

int dequeue() {
    pthread_mutex_lock(&mutex_queue);  // LOCK queue
    if (front == rear) {
        pthread_mutex_unlock(&mutex_queue);
        return -1;
    }
    int x = order_queue[front++];
    pthread_mutex_unlock(&mutex_queue);
    return x;
}

// ------------------------------
// THREAD: Payment Receiver
// ------------------------------
void* payment_thread(void* arg) {
    for (int i = 0; i < TOTAL_ORDER; i++) {
        pthread_mutex_lock(&mutex_payment);
        payment_status[i] = 1;  // payment OK
        
        // Calculate revenue
        int amount = 25000 + (rand() % 25000); // 25k-50k
        total_revenue += amount;
        
        printf("[Payment] Order %d dibayar (Rp %d) - Total Revenue: Rp %d\n", i, amount, total_revenue);
        pthread_cond_broadcast(&cond_payment); // signal payment OK
        pthread_mutex_unlock(&mutex_payment);
        usleep(10000);
    }
    
    // Final broadcast to wake up all waiting threads
    pthread_mutex_lock(&mutex_payment);
    pthread_cond_broadcast(&cond_payment);
    pthread_mutex_unlock(&mutex_payment);
    
    return NULL;
}

// ------------------------------
// THREAD: Kitchen (5 chefs)
// ------------------------------
void* kitchen_thread(void* arg) {
    int worker_id = *(int*)arg;

    while (1) {
        // Check termination condition first
        pthread_mutex_lock(&mutex_payment);
        if (completed >= TOTAL_ORDER) {
            pthread_mutex_unlock(&mutex_payment);
            break;
        }
        pthread_mutex_unlock(&mutex_payment);

        int orderID = dequeue();
        if (orderID == -1) {
            usleep(10000); // Wait 10ms before retry
            continue;
        }

        // TUNGGU payment OK
        pthread_mutex_lock(&mutex_payment);
        while (payment_status[orderID] == 0 && completed < TOTAL_ORDER)
            pthread_cond_wait(&cond_payment, &mutex_payment);
        
        // Double check after waking up
        if (completed >= TOTAL_ORDER) {
            pthread_mutex_unlock(&mutex_payment);
            break;
        }
        pthread_mutex_unlock(&mutex_payment);
        
        // Record start time (FCFS: first available worker gets the task)
        pthread_mutex_lock(&mutex_metrics);
        int current_time = (int)difftime(time(NULL), start_time_global);
        order_metrics[orderID].start_time = current_time;
        order_metrics[orderID].worker_id = worker_id;
        order_metrics[orderID].waiting_time = current_time - order_metrics[orderID].arrival_time;
        pthread_mutex_unlock(&mutex_metrics);
        
        printf("[Kitchen %d] Masak order %d (burst: %d detik)\n", 
               worker_id, orderID, burst_times[orderID]);
        
        // Simulate cooking with burst time
        sleep(burst_times[orderID]);

        // Record completion time
        pthread_mutex_lock(&mutex_metrics);
        current_time = (int)difftime(time(NULL), start_time_global);
        order_metrics[orderID].completion_time = current_time;
        order_metrics[orderID].turnaround_time = current_time - order_metrics[orderID].arrival_time;
        pthread_mutex_unlock(&mutex_metrics);

        // LOCK stok
        pthread_mutex_lock(&mutex_stock);
        stock -= 1;
        pthread_mutex_unlock(&mutex_stock);

        // SIGNAL order selesai
        pthread_mutex_lock(&mutex_payment);
        completed++;
        pthread_cond_broadcast(&cond_done); // bangunkan stock & print
        pthread_mutex_unlock(&mutex_payment);
    }
    return NULL;
}

// ------------------------------
// THREAD: Update Stock
// ------------------------------
void* stock_thread(void* arg) {
    int last_completed = 0;

    pthread_mutex_lock(&mutex_payment);
    while (completed < TOTAL_ORDER) {
        // Tunggu sampai ada order baru yang selesai
        while (completed == last_completed && completed < TOTAL_ORDER) {
            pthread_cond_wait(&cond_done, &mutex_payment);
        }

        if (completed > last_completed) {
            // Ada order baru yang selesai
            int current_stock;
            pthread_mutex_lock(&mutex_stock);
            current_stock = stock;
            pthread_mutex_unlock(&mutex_stock);

            printf("[Stock] Update stok... (current=%d, completed=%d)\n", current_stock, completed);
            last_completed = completed;
        }
    }
    pthread_mutex_unlock(&mutex_payment);
    return NULL;
}

// ------------------------------
// THREAD: Print Struk
// ------------------------------
void* print_thread(void* arg) {
    int last_completed = 0;

    pthread_mutex_lock(&mutex_payment);
    while (completed < TOTAL_ORDER) {
        // Tunggu sampai ada order baru yang selesai
        while (completed == last_completed && completed < TOTAL_ORDER) {
            pthread_cond_wait(&cond_done, &mutex_payment);
        }

        if (completed > last_completed) {
            printf("[Print] Cetak struk... (total selesai=%d)\n", completed);
            last_completed = completed;
        }
    }
    pthread_mutex_unlock(&mutex_payment);
    return NULL;
}

// ------------------------------
// MAIN — INIT + CREATE THREAD
// ------------------------------
int main() {
    srand(42);  // Fixed seed untuk perbandingan dengan race condition version
    start_time_global = time(NULL);
    
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_mutex_init(&mutex_stock, NULL);
    pthread_mutex_init(&mutex_payment, NULL);
    pthread_mutex_init(&mutex_metrics, NULL);

    pthread_cond_init(&cond_payment, NULL);
    pthread_cond_init(&cond_done, NULL);

    printf("========================================\n");
    printf("   FOODGO SYNCHRONIZED + SCHEDULER\n");
    printf("   Multi-Worker FCFS Scheduling\n");
    printf("========================================\n\n");

    // Generate burst times (1-10 detik)
    printf("Generating burst times for orders...\n");
    for (int i = 0; i < TOTAL_ORDER; i++) {
        burst_times[i] = 1 + (rand() % 10);
    }
    printf("\n");

    // Inisialisasi status payment dan metrics
    for (int i = 0; i < TOTAL_ORDER; i++) {
        payment_status[i] = 0;
        order_metrics[i].order_id = i;
        order_metrics[i].burst_time = burst_times[i];
        order_metrics[i].arrival_time = 0;  // All arrive at time 0
        order_metrics[i].start_time = -1;
        order_metrics[i].completion_time = -1;
        order_metrics[i].waiting_time = 0;
        order_metrics[i].turnaround_time = 0;
        order_metrics[i].worker_id = -1;
    }

    // Generate 30 orders
    printf("--- Generating Orders ---\n");
    for (int i = 0; i < TOTAL_ORDER; i++) {
        enqueue(i);  // push ke queue
        printf("[OrderReceiver] Order %d masuk (burst: %d detik)\n", i, burst_times[i]);
    }
    printf("\n");

    pthread_t payThread, stockThread, printThread;
    pthread_t kitchen[KITCHEN_COUNT];
    int worker_ids[KITCHEN_COUNT];

    printf("--- Starting Threads ---\n");
    pthread_create(&payThread, NULL, payment_thread, NULL);
    pthread_create(&stockThread, NULL, stock_thread, NULL);
    pthread_create(&printThread, NULL, print_thread, NULL);

    for (int i = 0; i < KITCHEN_COUNT; i++) {
        worker_ids[i] = i + 1;  // Worker ID 1-5
        pthread_create(&kitchen[i], NULL, kitchen_thread, &worker_ids[i]);
    }
    printf("\n");

    pthread_join(payThread, NULL);
    
    // Broadcast to wake up all waiting threads
    pthread_cond_broadcast(&cond_payment);
    pthread_cond_broadcast(&cond_done);
    
    // Wait for all kitchen threads to finish
    for (int i = 0; i < KITCHEN_COUNT; i++)
        pthread_join(kitchen[i], NULL);
    
    pthread_join(stockThread, NULL);
    pthread_join(printThread, NULL);

    printf("\n========================================\n");
    printf("         FINAL STATISTICS\n");
    printf("========================================\n");
    printf("Total Orders Processed: %d\n", completed);
    printf("Expected Orders: %d\n", TOTAL_ORDER);
    printf("Total Revenue: Rp %d\n", total_revenue);
    printf("Final Stock: %d\n", stock);
    printf("Expected Stock: %d\n", 100 - TOTAL_ORDER);
    printf("========================================\n\n");

    // Print Multi-Worker FCFS Scheduling Results
    printf("========================================\n");
    printf("   MULTI-WORKER FCFS SCHEDULING\n");
    printf("   (%d Kitchen Workers)\n", KITCHEN_COUNT);
    printf("========================================\n\n");

    printf("Order   Burst   Worker  Waiting  Turnaround\n");
    printf("-----   -----   ------  -------  ----------\n");
    
    int total_waiting = 0;
    int total_turnaround = 0;
    int max_completion = 0;
    
    for (int i = 0; i < TOTAL_ORDER; i++) {
        printf("%5d   %5d   %6d  %7d  %10d\n",
               order_metrics[i].order_id,
               order_metrics[i].burst_time,
               order_metrics[i].worker_id,
               order_metrics[i].waiting_time,
               order_metrics[i].turnaround_time);
        
        total_waiting += order_metrics[i].waiting_time;
        total_turnaround += order_metrics[i].turnaround_time;
        
        if (order_metrics[i].completion_time > max_completion) {
            max_completion = order_metrics[i].completion_time;
        }
    }
    
    printf("\n========================================\n");
    printf("Total waktu eksekusi: %d detik\n", max_completion);
    printf("Rata-rata waiting time   : %.2f detik\n", (float)total_waiting / TOTAL_ORDER);
    printf("Rata-rata turnaround time: %.2f detik\n", (float)total_turnaround / TOTAL_ORDER);
    printf("========================================\n\n");

    printf("KESIMPULAN:\n");
    printf("- Algoritma: FCFS (First Come First Served)\n");
    printf("- Multi-Worker: %d kitchen workers paralel\n", KITCHEN_COUNT);
    printf("- Order dikerjakan oleh worker pertama yang available\n");
    printf("- Waiting time dan turnaround time lebih rendah\n");
    printf("  dibanding single-worker karena paralelisasi\n");
    printf("- Throughput meningkat dengan multi-worker\n");
    printf("========================================\n");
    
    pthread_mutex_destroy(&mutex_queue);
    pthread_mutex_destroy(&mutex_stock);
    pthread_mutex_destroy(&mutex_payment);
    pthread_mutex_destroy(&mutex_metrics);
    pthread_cond_destroy(&cond_payment);
    pthread_cond_destroy(&cond_done);
    
    return 0;
}
