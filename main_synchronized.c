#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TOTAL_ORDER 30
#define KITCHEN_COUNT 5

int order_queue[100];
int front = 0, rear = 0;

int payment_status[TOTAL_ORDER];
int stock = 100; // contoh stok bahan
int total_revenue = 0; // total pendapatan

pthread_mutex_t mutex_queue;
pthread_mutex_t mutex_stock;
pthread_mutex_t mutex_payment;

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
        
        printf("[Kitchen] Masak order %d\n", orderID);
        sleep(1);

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
    
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_mutex_init(&mutex_stock, NULL);
    pthread_mutex_init(&mutex_payment, NULL);

    pthread_cond_init(&cond_payment, NULL);
    pthread_cond_init(&cond_done, NULL);

    // Inisialisasi status payment
    for (int i = 0; i < TOTAL_ORDER; i++) {
        payment_status[i] = 0;
    }

    // Generate 30 orders
    for (int i = 0; i < TOTAL_ORDER; i++) {
        enqueue(i);  // push ke queue
        printf("[OrderReceiver] Order %d masuk\n", i);
    }

    pthread_t payThread, stockThread, printThread;
    pthread_t kitchen[KITCHEN_COUNT];

    pthread_create(&payThread, NULL, payment_thread, NULL);
    pthread_create(&stockThread, NULL, stock_thread, NULL);
    pthread_create(&printThread, NULL, print_thread, NULL);

    for (int i = 0; i < KITCHEN_COUNT; i++)
        pthread_create(&kitchen[i], NULL, kitchen_thread, NULL);

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
    printf("========================================\n");
    
    pthread_mutex_destroy(&mutex_queue);
    pthread_mutex_destroy(&mutex_stock);
    pthread_mutex_destroy(&mutex_payment);
    pthread_cond_destroy(&cond_payment);
    pthread_cond_destroy(&cond_done);
    
    return 0;
}
