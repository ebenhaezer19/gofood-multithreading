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
        printf("[Payment] Order %d dibayar\n", i);
        pthread_cond_broadcast(&cond_payment); // signal payment OK
        pthread_mutex_unlock(&mutex_payment);
        usleep(10000);
    }
    return NULL;
}

// ------------------------------
// THREAD: Kitchen (5 chefs)
// ------------------------------
void* kitchen_thread(void* arg) {

    while (1) {

        int orderID = dequeue();
        if (orderID == -1) continue; // queue kosong

        // TUNGGU payment OK
        pthread_mutex_lock(&mutex_payment);
        while (payment_status[orderID] == 0)
            pthread_cond_wait(&cond_payment, &mutex_payment);
        pthread_mutex_unlock(&mutex_payment);

        printf("[Kitchen] Masak order %d\n", orderID);
        sleep(1);

        // LOCK stok
        pthread_mutex_lock(&mutex_stock);
        stock -= 1;
        pthread_mutex_unlock(&mutex_stock);

        // SIGNAL order selesai
        pthread_cond_signal(&cond_done);

        pthread_mutex_lock(&mutex_payment);
        completed++;
        pthread_mutex_unlock(&mutex_payment);

        if (completed >= TOTAL_ORDER) break;
    }
    return NULL;
}

// ------------------------------
// THREAD: Update Stock
// ------------------------------
void* stock_thread(void* arg) {
    while (1) {
        pthread_cond_wait(&cond_done, &mutex_payment);
        printf("[Stock] Update stok... (current=%d)\n", stock);
        if (completed >= TOTAL_ORDER) break;
    }
    return NULL;
}

// ------------------------------
// THREAD: Print Struk
// ------------------------------
void* print_thread(void* arg) {
    while (1) {
        pthread_cond_wait(&cond_done, &mutex_payment);
        printf("[Print] Cetak struk...\n");
        if (completed >= TOTAL_ORDER) break;
    }
    return NULL;
}

// ------------------------------
// MAIN — INIT + CREATE THREAD
// ------------------------------
int main() {
    pthread_mutex_init(&mutex_queue, NULL);
    pthread_mutex_init(&mutex_stock, NULL);
    pthread_mutex_init(&mutex_payment, NULL);

    pthread_cond_init(&cond_payment, NULL);
    pthread_cond_init(&cond_done, NULL);

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
    pthread_join(stockThread, NULL);
    pthread_join(printThread, NULL);

    for (int i = 0; i < KITCHEN_COUNT; i++)
        pthread_join(kitchen[i], NULL);

    printf("Semua order selesai.\n");
    return 0;
}
