// ============================================================================
// RACE CONDITION DEMONSTRATION - GO-FOOD MULTITHREADING
// Versi TANPA Synchronization untuk menunjukkan race condition
// ============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define TOTAL_ORDER 30
#define KITCHEN_COUNT 5

// ============================================================================
// SHARED VARIABLES (TANPA PROTECTION - RACE CONDITION!)
// ============================================================================
int order_queue[100];
int front = 0, rear = 0;

int payment_status[TOTAL_ORDER];
int stock = 100; // contoh stok bahan

int completed = 0;
int total_revenue = 0;

// ============================================================================
// Queue functions (TANPA MUTEX - RACE CONDITION!)
// ============================================================================
void enqueue(int orderID) {
    // TIDAK ADA LOCK - RACE CONDITION BISA TERJADI!
    order_queue[rear++] = orderID;
    printf("[OrderReceiver] Order %d masuk ke queue (rear=%d)\n", orderID, rear);
}

int dequeue() {
    // TIDAK ADA LOCK - RACE CONDITION BISA TERJADI!
    if (front == rear) {
        return -1;
    }
    int x = order_queue[front++];
    return x;
}

// ============================================================================
// THREAD 1: Payment Receiver (TANPA SYNCHRONIZATION)
// ============================================================================
void* payment_thread(void* arg) {
    printf("[Payment Thread] Started\n");
    
    for (int i = 0; i < TOTAL_ORDER; i++) {
        // TIDAK ADA MUTEX - RACE CONDITION!
        payment_status[i] = 1;  // payment OK
        
        // Simulasi processing payment
        int amount = 25000 + (rand() % 25000); // 25k-50k
        
        // RACE CONDITION: Multiple threads bisa akses total_revenue bersamaan!
        total_revenue += amount;
        
        printf("[Payment] Order %d dibayar (Rp %d) - Total Revenue: Rp %d\n", 
               i, amount, total_revenue);
        
        usleep(10000); // 10ms delay
    }
    
    printf("[Payment Thread] Completed\n");
    return NULL;
}

// ============================================================================
// THREAD 2: Kitchen (5 chefs) (TANPA SYNCHRONIZATION)
// ============================================================================
void* kitchen_thread(void* arg) {
    int chef_id = *(int*)arg;
    printf("[Kitchen %d] Started\n", chef_id);
    
    while (1) {
        // RACE CONDITION: Multiple threads dequeue bersamaan!
        int orderID = dequeue();
        
        if (orderID == -1) {
            usleep(5000); // Wait sedikit
            
            // RACE CONDITION: Check completed tanpa lock!
            if (completed >= TOTAL_ORDER) break;
            continue;
        }
        
        // RACE CONDITION: Bisa akses payment_status yang belum ready!
        if (payment_status[orderID] == 0) {
            printf("[Kitchen %d] WARNING: Order %d belum dibayar tapi diproses!\n", 
                   chef_id, orderID);
        }
        
        printf("[Kitchen %d] Masak order %d\n", chef_id, orderID);
        usleep(50000 + (rand() % 50000)); // 50-100ms cooking time
        
        // RACE CONDITION: Update stock tanpa lock!
        stock -= 1;
        printf("[Kitchen %d] Order %d selesai - Stock: %d\n", chef_id, orderID, stock);
        
        // RACE CONDITION: Increment completed tanpa lock!
        completed++;
        
        if (completed >= TOTAL_ORDER) break;
    }
    
    printf("[Kitchen %d] Completed\n", chef_id);
    return NULL;
}

// ============================================================================
// THREAD 3: Stock Manager (TANPA SYNCHRONIZATION)
// ============================================================================
void* stock_thread(void* arg) {
    printf("[Stock Manager] Started\n");
    
    int last_completed = 0;
    
    while (1) {
        usleep(100000); // Check setiap 100ms
        
        // RACE CONDITION: Read completed tanpa lock!
        if (completed > last_completed) {
            printf("[Stock Manager] Update stok... (current=%d, completed=%d)\n", 
                   stock, completed);
            
            // RACE CONDITION: Bisa baca stock yang sedang di-update!
            if (stock < 20) {
                printf("[Stock Manager] WARNING: Stock rendah! (%d remaining)\n", stock);
            }
            
            last_completed = completed;
        }
        
        if (completed >= TOTAL_ORDER) break;
    }
    
    printf("[Stock Manager] Completed\n");
    return NULL;
}

// ============================================================================
// THREAD 4: Print Struk (TANPA SYNCHRONIZATION)
// ============================================================================
void* print_thread(void* arg) {
    printf("[Print Thread] Started\n");
    
    int last_completed = 0;
    
    while (1) {
        usleep(80000); // Check setiap 80ms
        
        // RACE CONDITION: Read completed tanpa lock!
        if (completed > last_completed) {
            printf("[Print] Cetak struk untuk order yang selesai... (total=%d)\n", completed);
            last_completed = completed;
        }
        
        if (completed >= TOTAL_ORDER) break;
    }
    
    printf("[Print Thread] Completed\n");
    return NULL;
}

// ============================================================================
// MAIN FUNCTION
// ============================================================================
int main() {
    printf("========================================\n");
    printf("   GO-FOOD RACE CONDITION DEMO\n");
    printf("   (TANPA SYNCHRONIZATION)\n");
    printf("========================================\n\n");
    
    printf("⚠️  WARNING: Program ini TIDAK menggunakan mutex/semaphore\n");
    printf("⚠️  Race condition AKAN terjadi!\n");
    printf("⚠️  Data mungkin TIDAK konsisten!\n\n");
    
    srand(time(NULL));
    
    // Initialize payment status
    for (int i = 0; i < TOTAL_ORDER; i++) {
        payment_status[i] = 0;
    }
    
    // Generate 30 orders dan masukkan ke queue
    printf("--- Generating Orders ---\n");
    for (int i = 0; i < TOTAL_ORDER; i++) {
        enqueue(i);
        usleep(5000); // 5ms delay antar order
    }
    printf("\n");
    
    // Create threads
    pthread_t payThread, stockThread, printThread;
    pthread_t kitchen[KITCHEN_COUNT];
    int chef_ids[KITCHEN_COUNT];
    
    printf("--- Starting Threads ---\n");
    
    // Start payment thread
    pthread_create(&payThread, NULL, payment_thread, NULL);
    
    // Start kitchen threads (5 chefs)
    for (int i = 0; i < KITCHEN_COUNT; i++) {
        chef_ids[i] = i + 1;
        pthread_create(&kitchen[i], NULL, kitchen_thread, &chef_ids[i]);
    }
    
    // Start stock manager thread
    pthread_create(&stockThread, NULL, stock_thread, NULL);
    
    // Start print thread
    pthread_create(&printThread, NULL, print_thread, NULL);
    
    printf("\n--- Processing Orders ---\n\n");
    
    // Wait for all threads to complete
    pthread_join(payThread, NULL);
    
    for (int i = 0; i < KITCHEN_COUNT; i++) {
        pthread_join(kitchen[i], NULL);
    }
    
    pthread_join(stockThread, NULL);
    pthread_join(printThread, NULL);
    
    // Print final statistics
    printf("\n========================================\n");
    printf("         FINAL STATISTICS\n");
    printf("========================================\n");
    printf("Total Orders Processed: %d\n", completed);
    printf("Expected Orders: %d\n", TOTAL_ORDER);
    printf("Total Revenue: Rp %d\n", total_revenue);
    printf("Final Stock: %d\n", stock);
    printf("Expected Stock: %d\n", 100 - TOTAL_ORDER);
    printf("========================================\n\n");
    
    // Analisis race condition
    printf("--- RACE CONDITION ANALYSIS ---\n");
    
    if (completed != TOTAL_ORDER) {
        printf("❌ RACE CONDITION DETECTED!\n");
        printf("   Expected: %d orders, Got: %d orders\n", TOTAL_ORDER, completed);
        printf("   Difference: %d orders\n", TOTAL_ORDER - completed);
    } else {
        printf("⚠️  Order count correct, but may have race conditions\n");
    }
    
    int expected_stock = 100 - TOTAL_ORDER;
    if (stock != expected_stock) {
        printf("❌ STOCK INCONSISTENCY!\n");
        printf("   Expected: %d, Got: %d\n", expected_stock, stock);
        printf("   Difference: %d\n", expected_stock - stock);
    }
    
    printf("\n💡 KESIMPULAN:\n");
    printf("   Tanpa synchronization (mutex/semaphore),\n");
    printf("   multiple threads mengakses shared variables\n");
    printf("   secara bersamaan, menyebabkan:\n");
    printf("   - Lost updates\n");
    printf("   - Data inconsistency\n");
    printf("   - Incorrect calculations\n");
    printf("\n");
    
    return 0;
}
