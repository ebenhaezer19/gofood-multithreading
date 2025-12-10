// COBA2 DARI AI (BUAT SEMUA THREAD DISINI)

// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>
// #include <unistd.h>
// #include <string.h>
// #include <time.h>
// #include <semaphore.h>
// #include <sys/mman.h>

// // ============================================================================
// // CONSTANTS AND CONFIGURATION
// // ============================================================================
// #define NUM_ORDERS 30
// #define NUM_THREADS 4
// #define MAX_STOCK 100
// #define LOW_STOCK_THRESHOLD 20

// // ============================================================================
// // DATA STRUCTURES
// // ============================================================================

// typedef enum {
//     ORDER_PENDING,
//     ORDER_PAYMENT_CONFIRMED,
//     ORDER_COOKING,
//     ORDER_COMPLETED,
//     ORDER_FAILED
// } OrderStatus;

// typedef struct {
//     int order_id;
//     char customer_name[50];
//     char food_item[50];
//     double price;
//     OrderStatus status;
//     int payment_confirmed;
//     int cooking_done;
//     time_t timestamp;
// } Order;

// typedef struct {
//     int item_id;
//     char item_name[50];
//     int quantity;
// } StockItem;

// // ============================================================================
// // SHARED MEMORY (Race Condition akan terjadi di sini)
// // ============================================================================

// // Shared resources yang akan menyebabkan race condition
// Order *shared_orders;           // Array of orders (shared memory)
// int order_count = 0;            // Counter untuk order yang masuk
// int payment_processed = 0;      // Counter payment yang diproses
// int kitchen_processed = 0;      // Counter kitchen yang diproses
// StockItem *stock_inventory;     // Inventory stock
// int total_revenue = 0;          // Total pendapatan

// // ============================================================================
// // SYNCHRONIZATION PRIMITIVES
// // ============================================================================

// // Mutex untuk melindungi shared resources
// pthread_mutex_t order_mutex;
// pthread_mutex_t payment_mutex;
// pthread_mutex_t kitchen_mutex;
// pthread_mutex_t stock_mutex;
// pthread_mutex_t revenue_mutex;

// // Semaphores untuk sequential dependency
// sem_t order_received_sem;       // Signal bahwa order sudah diterima
// sem_t payment_confirmed_sem;    // Signal bahwa payment sudah dikonfirmasi
// sem_t cooking_done_sem;         // Signal bahwa cooking sudah selesai

// // Condition variables untuk thread coordination
// pthread_cond_t order_ready_cond;
// pthread_cond_t payment_ready_cond;
// pthread_cond_t kitchen_ready_cond;

// // Flag untuk demonstrasi race condition
// int enable_synchronization = 0;  // 0 = race condition, 1 = synchronized

// // ============================================================================
// // MEMORY MAPPING STRUCTURE (untuk analisis)
// // ============================================================================

// typedef struct {
//     void *order_memory_start;
//     void *order_memory_end;
//     void *stock_memory_start;
//     void *stock_memory_end;
//     size_t order_memory_size;
//     size_t stock_memory_size;
//     int thread_id;
//     void *thread_stack_start;
// } MemoryMap;

// MemoryMap memory_maps[NUM_THREADS];

// // ============================================================================
// // UTILITY FUNCTIONS
// // ============================================================================

// void print_memory_map() {
//     printf("\n========== MEMORY MAPPING ANALYSIS ==========\n");
//     printf("Shared Order Memory: %p - %p (Size: %zu bytes)\n", 
//            shared_orders, 
//            (void*)((char*)shared_orders + sizeof(Order) * NUM_ORDERS),
//            sizeof(Order) * NUM_ORDERS);
//     printf("Shared Stock Memory: %p - %p (Size: %zu bytes)\n", 
//            stock_inventory, 
//            (void*)((char*)stock_inventory + sizeof(StockItem) * 5),
//            sizeof(StockItem) * 5);
//     printf("\nShared Variables (Race Condition Points):\n");
//     printf("  - order_count: %p\n", (void*)&order_count);
//     printf("  - payment_processed: %p\n", (void*)&payment_processed);
//     printf("  - kitchen_processed: %p\n", (void*)&kitchen_processed);
//     printf("  - total_revenue: %p\n", (void*)&total_revenue);
//     printf("=============================================\n\n");
// }

// void init_stock() {
//     stock_inventory = mmap(NULL, sizeof(StockItem) * 5, 
//                           PROT_READ | PROT_WRITE,
//                           MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
//     strcpy(stock_inventory[0].item_name, "Nasi Goreng");
//     stock_inventory[0].quantity = 50;
//     stock_inventory[0].item_id = 1;
    
//     strcpy(stock_inventory[1].item_name, "Mie Goreng");
//     stock_inventory[1].quantity = 40;
//     stock_inventory[1].item_id = 2;
    
//     strcpy(stock_inventory[2].item_name, "Ayam Geprek");
//     stock_inventory[2].quantity = 30;
//     stock_inventory[2].item_id = 3;
    
//     strcpy(stock_inventory[3].item_name, "Sate Ayam");
//     stock_inventory[3].quantity = 25;
//     stock_inventory[3].item_id = 4;
    
//     strcpy(stock_inventory[4].item_name, "Bakso");
//     stock_inventory[4].quantity = 35;
//     stock_inventory[4].item_id = 5;
// }

// void generate_random_order(Order *order, int id) {
//     order->order_id = id;
//     sprintf(order->customer_name, "Customer_%d", id);
    
//     int food_choice = rand() % 5;
//     strcpy(order->food_item, stock_inventory[food_choice].item_name);
//     order->price = 15000 + (rand() % 35000); // 15k - 50k
//     order->status = ORDER_PENDING;
//     order->payment_confirmed = 0;
//     order->cooking_done = 0;
//     order->timestamp = time(NULL);
// }

// // ============================================================================
// // THREAD 1: ORDER RECEIVER
// // ============================================================================

// void* order_receiver_thread(void* arg) {
//     int thread_id = *(int*)arg;
//     printf("[ORDER RECEIVER] Thread %d started\n", thread_id);
    
//     // Record memory mapping
//     memory_maps[0].thread_id = thread_id;
//     memory_maps[0].order_memory_start = shared_orders;
//     memory_maps[0].order_memory_end = (void*)((char*)shared_orders + sizeof(Order) * NUM_ORDERS);
//     memory_maps[0].order_memory_size = sizeof(Order) * NUM_ORDERS;
    
//     for (int i = 0; i < NUM_ORDERS; i++) {
//         // Simulate order arrival
//         usleep(100000); // 100ms delay
        
//         if (enable_synchronization) {
//             pthread_mutex_lock(&order_mutex);
//         }
        
//         // CRITICAL SECTION - Race condition bisa terjadi di sini
//         Order new_order;
//         generate_random_order(&new_order, order_count + 1);
//         shared_orders[order_count] = new_order;
        
//         printf("[ORDER] Order #%d received: %s orders %s (Rp %.0f)\n",
//                new_order.order_id, new_order.customer_name, 
//                new_order.food_item, new_order.price);
        
//         order_count++;
        
//         if (enable_synchronization) {
//             pthread_mutex_unlock(&order_mutex);
//             pthread_cond_signal(&order_ready_cond);
//             sem_post(&order_received_sem);
//         }
//     }
    
//     printf("[ORDER RECEIVER] Thread %d completed. Total orders: %d\n", thread_id, order_count);
//     return NULL;
// }

// // ============================================================================
// // THREAD 2: PAYMENT PROCESSOR
// // ============================================================================

// void* payment_processor_thread(void* arg) {
//     int thread_id = *(int*)arg;
//     printf("[PAYMENT PROCESSOR] Thread %d started\n", thread_id);
    
//     // Record memory mapping
//     memory_maps[1].thread_id = thread_id;
//     memory_maps[1].order_memory_start = shared_orders;
    
//     int processed = 0;
    
//     while (processed < NUM_ORDERS) {
//         if (enable_synchronization) {
//             // Wait untuk order baru
//             sem_wait(&order_received_sem);
//             pthread_mutex_lock(&payment_mutex);
//         }
        
//         // CRITICAL SECTION - Race condition pada payment_processed
//         if (payment_processed < order_count) {
//             Order *order = &shared_orders[payment_processed];
            
//             // Simulate payment processing
//             usleep(50000); // 50ms
            
//             order->payment_confirmed = 1;
//             order->status = ORDER_PAYMENT_CONFIRMED;
            
//             // Update revenue (race condition point!)
//             if (enable_synchronization) {
//                 pthread_mutex_lock(&revenue_mutex);
//             }
//             total_revenue += order->price;
//             if (enable_synchronization) {
//                 pthread_mutex_unlock(&revenue_mutex);
//             }
            
//             printf("[PAYMENT] Order #%d payment confirmed: Rp %.0f (Total: Rp %d)\n",
//                    order->order_id, order->price, total_revenue);
            
//             payment_processed++;
//             processed++;
            
//             if (enable_synchronization) {
//                 pthread_mutex_unlock(&payment_mutex);
//                 pthread_cond_signal(&payment_ready_cond);
//                 sem_post(&payment_confirmed_sem);
//             }
//         } else {
//             if (enable_synchronization) {
//                 pthread_mutex_unlock(&payment_mutex);
//             }
//             usleep(10000);
//         }
//     }
    
//     printf("[PAYMENT PROCESSOR] Thread %d completed. Processed: %d payments\n", 
//            thread_id, processed);
//     return NULL;
// }

// // ============================================================================
// // THREAD 3: KITCHEN PROCESSOR
// // ============================================================================

// void* kitchen_processor_thread(void* arg) {
//     int thread_id = *(int*)arg;
//     printf("[KITCHEN] Thread %d started\n", thread_id);
    
//     // Record memory mapping
//     memory_maps[2].thread_id = thread_id;
//     memory_maps[2].order_memory_start = shared_orders;
    
//     int processed = 0;
    
//     while (processed < NUM_ORDERS) {
//         if (enable_synchronization) {
//             // Wait untuk payment confirmation
//             sem_wait(&payment_confirmed_sem);
//             pthread_mutex_lock(&kitchen_mutex);
//         }
        
//         // CRITICAL SECTION - Race condition pada kitchen_processed
//         if (kitchen_processed < payment_processed) {
//             Order *order = &shared_orders[kitchen_processed];
            
//             if (order->payment_confirmed) {
//                 // Simulate cooking
//                 usleep(150000); // 150ms cooking time
                
//                 order->cooking_done = 1;
//                 order->status = ORDER_COOKING;
                
//                 printf("[KITCHEN] Order #%d cooking: %s for %s\n",
//                        order->order_id, order->food_item, order->customer_name);
                
//                 kitchen_processed++;
//                 processed++;
                
//                 if (enable_synchronization) {
//                     pthread_mutex_unlock(&kitchen_mutex);
//                     pthread_cond_signal(&kitchen_ready_cond);
//                     sem_post(&cooking_done_sem);
//                 }
//             } else {
//                 if (enable_synchronization) {
//                     pthread_mutex_unlock(&kitchen_mutex);
//                 }
//             }
//         } else {
//             if (enable_synchronization) {
//                 pthread_mutex_unlock(&kitchen_mutex);
//             }
//             usleep(10000);
//         }
//     }
    
//     printf("[KITCHEN] Thread %d completed. Cooked: %d orders\n", thread_id, processed);
//     return NULL;
// }

// // ============================================================================
// // THREAD 4: STOCK MANAGER
// // ============================================================================

// void* stock_manager_thread(void* arg) {
//     int thread_id = *(int*)arg;
//     printf("[STOCK MANAGER] Thread %d started\n", thread_id);
    
//     // Record memory mapping
//     memory_maps[3].thread_id = thread_id;
//     memory_maps[3].stock_memory_start = stock_inventory;
//     memory_maps[3].stock_memory_end = (void*)((char*)stock_inventory + sizeof(StockItem) * 5);
    
//     int processed = 0;
    
//     while (processed < NUM_ORDERS) {
//         if (enable_synchronization) {
//             // Wait untuk cooking done
//             sem_wait(&cooking_done_sem);
//             pthread_mutex_lock(&stock_mutex);
//         }
        
//         // CRITICAL SECTION - Race condition pada stock inventory
//         if (processed < kitchen_processed) {
//             Order *order = &shared_orders[processed];
            
//             if (order->cooking_done) {
//                 // Update stock
//                 for (int i = 0; i < 5; i++) {
//                     if (strcmp(stock_inventory[i].item_name, order->food_item) == 0) {
//                         stock_inventory[i].quantity--;
                        
//                         printf("[STOCK] Order #%d completed. %s stock: %d remaining\n",
//                                order->order_id, stock_inventory[i].item_name, 
//                                stock_inventory[i].quantity);
                        
//                         if (stock_inventory[i].quantity < LOW_STOCK_THRESHOLD) {
//                             printf("[STOCK] WARNING: %s stock is low! (%d remaining)\n",
//                                    stock_inventory[i].item_name, stock_inventory[i].quantity);
//                         }
//                         break;
//                     }
//                 }
                
//                 order->status = ORDER_COMPLETED;
//                 processed++;
//             }
            
//             if (enable_synchronization) {
//                 pthread_mutex_unlock(&stock_mutex);
//             }
//         } else {
//             if (enable_synchronization) {
//                 pthread_mutex_unlock(&stock_mutex);
//             }
//             usleep(10000);
//         }
//     }
    
//     printf("[STOCK MANAGER] Thread %d completed. Managed: %d orders\n", 
//            thread_id, processed);
//     return NULL;
// }

// // ============================================================================
// // INITIALIZATION AND CLEANUP
// // ============================================================================

// void init_synchronization() {
//     pthread_mutex_init(&order_mutex, NULL);
//     pthread_mutex_init(&payment_mutex, NULL);
//     pthread_mutex_init(&kitchen_mutex, NULL);
//     pthread_mutex_init(&stock_mutex, NULL);
//     pthread_mutex_init(&revenue_mutex, NULL);
    
//     sem_init(&order_received_sem, 0, 0);
//     sem_init(&payment_confirmed_sem, 0, 0);
//     sem_init(&cooking_done_sem, 0, 0);
    
//     pthread_cond_init(&order_ready_cond, NULL);
//     pthread_cond_init(&payment_ready_cond, NULL);
//     pthread_cond_init(&kitchen_ready_cond, NULL);
// }

// void cleanup_synchronization() {
//     pthread_mutex_destroy(&order_mutex);
//     pthread_mutex_destroy(&payment_mutex);
//     pthread_mutex_destroy(&kitchen_mutex);
//     pthread_mutex_destroy(&stock_mutex);
//     pthread_mutex_destroy(&revenue_mutex);
    
//     sem_destroy(&order_received_sem);
//     sem_destroy(&payment_confirmed_sem);
//     sem_destroy(&cooking_done_sem);
    
//     pthread_cond_destroy(&order_ready_cond);
//     pthread_cond_destroy(&payment_ready_cond);
//     pthread_cond_destroy(&kitchen_ready_cond);
// }

// // ============================================================================
// // MAIN FUNCTION
// // ============================================================================

// int main(int argc, char *argv[]) {
//     printf("========================================\n");
//     printf("   GO-FOOD MULTITHREADING SYSTEM\n");
//     printf("========================================\n\n");
    
//     // Parse command line arguments
//     if (argc > 1 && strcmp(argv[1], "--sync") == 0) {
//         enable_synchronization = 1;
//         printf("MODE: SYNCHRONIZED (Race condition handled)\n\n");
//     } else {
//         enable_synchronization = 0;
//         printf("MODE: UNSYNCHRONIZED (Race condition demonstration)\n\n");
//     }
    
//     // Seed random
//     srand(time(NULL));
    
//     // Allocate shared memory using mmap
//     shared_orders = mmap(NULL, sizeof(Order) * NUM_ORDERS, 
//                         PROT_READ | PROT_WRITE,
//                         MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    
//     if (shared_orders == MAP_FAILED) {
//         perror("mmap failed");
//         return 1;
//     }
    
//     // Initialize stock
//     init_stock();
    
//     // Initialize synchronization primitives
//     init_synchronization();
    
//     // Print memory mapping
//     print_memory_map();
    
//     // Create threads
//     pthread_t threads[NUM_THREADS];
//     int thread_ids[NUM_THREADS] = {1, 2, 3, 4};
    
//     printf("Starting threads...\n\n");
    
//     time_t start_time = time(NULL);
    
//     pthread_create(&threads[0], NULL, order_receiver_thread, &thread_ids[0]);
//     pthread_create(&threads[1], NULL, payment_processor_thread, &thread_ids[1]);
//     pthread_create(&threads[2], NULL, kitchen_processor_thread, &thread_ids[2]);
//     pthread_create(&threads[3], NULL, stock_manager_thread, &thread_ids[3]);
    
//     // Wait for all threads to complete
//     for (int i = 0; i < NUM_THREADS; i++) {
//         pthread_join(threads[i], NULL);
//     }
    
//     time_t end_time = time(NULL);
    
//     // Print final statistics
//     printf("\n========================================\n");
//     printf("         FINAL STATISTICS\n");
//     printf("========================================\n");
//     printf("Total Orders Received: %d\n", order_count);
//     printf("Total Payments Processed: %d\n", payment_processed);
//     printf("Total Kitchen Processed: %d\n", kitchen_processed);
//     printf("Total Revenue: Rp %d\n", total_revenue);
//     printf("Execution Time: %ld seconds\n", end_time - start_time);
//     printf("\nFinal Stock Inventory:\n");
//     for (int i = 0; i < 5; i++) {
//         printf("  - %s: %d\n", stock_inventory[i].item_name, stock_inventory[i].quantity);
//     }
//     printf("========================================\n\n");
    
//     // Cleanup
//     cleanup_synchronization();
//     munmap(shared_orders, sizeof(Order) * NUM_ORDERS);
//     munmap(stock_inventory, sizeof(StockItem) * 5);
    
//     return 0;
// }



