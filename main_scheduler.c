#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_TASK 30
#define NUM_WORKER 5  // 5 koki di dapur

typedef struct {
    int id;
    int burst;           // lama masak / CPU burst time
    int start_time;      // waktu mulai dikerjakan
    int completion_time; // waktu selesai
    int waiting;          // waiting time
    int turnaround;      // turnaround time
} Task;

typedef struct {
    int worker_id;
    int current_task_id; // -1 jika idle
    int remaining_time;  // sisa waktu untuk task saat ini
} Worker;

// Utility: cetak ringkasan hasil scheduler
void print_result(const char* name, Task tasks[], int n, int total_time) {
    double avg_wait = 0.0, avg_tat = 0.0;
    printf("\n=== %s (Multi-Worker dengan %d Koki) ===\n", name, NUM_WORKER);
    printf("ID\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%2d\t%5d\t%7d\t%10d\n",
               tasks[i].id, tasks[i].burst, tasks[i].waiting, tasks[i].turnaround);
        avg_wait += tasks[i].waiting;
        avg_tat  += tasks[i].turnaround;
    }
    avg_wait /= n;
    avg_tat  /= n;

    printf("\nTotal waktu eksekusi: %d unit\n", total_time);
    printf("Rata-rata waiting time   : %.2f\n", avg_wait);
    printf("Rata-rata turnaround time: %.2f\n\n", avg_tat);
}

// Multi-Worker FCFS: 5 koki mengambil order dari queue FCFS secara paralel
void simulate_multiworker_fcfs(Task src[], int n) {
    Task tasks[NUM_TASK];
    Worker workers[NUM_WORKER];
    int queue[NUM_TASK];
    int queue_front = 0, queue_rear = 0;
    int completed = 0;
    int time = 0;
    
    // Inisialisasi tasks
    for (int i = 0; i < n; i++) {
        tasks[i] = src[i];
        tasks[i].start_time = -1;
        tasks[i].completion_time = -1;
        tasks[i].waiting = 0;
        tasks[i].turnaround = 0;
    }
    
    // Inisialisasi workers (semua idle)
    for (int i = 0; i < NUM_WORKER; i++) {
        workers[i].worker_id = i;
        workers[i].current_task_id = -1;
        workers[i].remaining_time = 0;
    }
    
    // Masukkan semua task ke queue FCFS
    for (int i = 0; i < n; i++) {
        queue[queue_rear++] = i;
    }
    
    printf("\nProses Multi-Worker FCFS (5 Koki):\n");
    printf("Time | ");
    for (int w = 0; w < NUM_WORKER; w++) {
        printf("Koki%d  ", w + 1);
    }
    printf("\n");
    printf("-----+-");
    for (int w = 0; w < NUM_WORKER; w++) {
        printf("------");
    }
    printf("\n");
    
    // Simulasi waktu diskrit
    while (completed < n) {
        // Update status setiap worker
        for (int w = 0; w < NUM_WORKER; w++) {
            if (workers[w].current_task_id != -1) {
                // Worker sedang mengerjakan task
                workers[w].remaining_time--;
                
                if (workers[w].remaining_time == 0) {
                    // Task selesai
                    int task_id = workers[w].current_task_id;
                    tasks[task_id].completion_time = time;
                    // Turnaround = completion_time - arrival_time (arrival_time = 0 untuk semua task)
                    tasks[task_id].turnaround = tasks[task_id].completion_time;
                    // Waiting = start_time - arrival_time (arrival_time = 0)
                    tasks[task_id].waiting = tasks[task_id].start_time;
                    
                    completed++;
                    workers[w].current_task_id = -1;
                }
            }
            
            // Jika worker idle, ambil task berikutnya dari queue
            if (workers[w].current_task_id == -1 && queue_front < queue_rear) {
                int task_id = queue[queue_front++];
                workers[w].current_task_id = task_id;
                workers[w].remaining_time = tasks[task_id].burst;
                tasks[task_id].start_time = time;
            }
        }
        
        // Print status setiap time unit (SEMUA time unit ditampilkan)
        printf("%4d | ", time);
        for (int w = 0; w < NUM_WORKER; w++) {
            if (workers[w].current_task_id != -1) {
                printf("T%2d   ", tasks[workers[w].current_task_id].id);
            } else {
                printf("idle  ");
            }
        }
        printf("\n");
        
        time++;
        
        // Safety: break jika terlalu lama (untuk menghindari infinite loop)
        if (time > 1000) break;
    }
    
    // Print baris terakhir (semua worker selesai)
    printf("%4d | ", time - 1);
    for (int w = 0; w < NUM_WORKER; w++) {
        printf("done  ");
    }
    printf("\n\n");
    
    // Hitung ulang waiting time dan turnaround time untuk memastikan konsistensi
    // (arrival_time = 0 untuk semua task, jadi waiting = start_time, turnaround = completion_time)
    for (int i = 0; i < n; i++) {
        if (tasks[i].start_time >= 0 && tasks[i].completion_time >= 0) {
            tasks[i].waiting = tasks[i].start_time;  // waiting = start_time - arrival_time (arrival = 0)
            tasks[i].turnaround = tasks[i].completion_time;  // turnaround = completion - arrival (arrival = 0)
        }
    }
    
    print_result("Multi-Worker FCFS", tasks, n, time - 1);
}

// FCFS Single Worker (untuk perbandingan)
void simulate_single_fcfs(Task src[], int n) {
    Task tasks[NUM_TASK];
    for (int i = 0; i < n; i++) {
        tasks[i] = src[i];
        tasks[i].waiting = 0;
        tasks[i].turnaround = 0;
    }

    int time = 0;
    printf("\nProses FCFS Single Worker :\n");
    printf("Time | Worker\n");
    printf("-----+-------\n");
    
    for (int i = 0; i < n; i++) {
        tasks[i].waiting = time;
        printf("%4d | T%2d\n", time, tasks[i].id);
        time += tasks[i].burst;
        tasks[i].turnaround = time;
    }
    printf("%4d | done\n\n", time);

    print_result("FCFS Single Worker", tasks, n, time);
}

int main() {
    Task base[NUM_TASK];

    // Seed random untuk konsistensi
    srand(42);

    // Generate 30 "order" dengan burst time acak 1–10 unit
    printf("========================================\n");
    printf("Simulasi CPU Scheduler Multi-Worker FCFS\n");
    printf("========================================\n");
    printf("Konfigurasi:\n");
    printf("- Jumlah Task/Order: %d\n", NUM_TASK);
    printf("- Jumlah Worker/Koki: %d\n", NUM_WORKER);
    printf("- Algoritma: FCFS (First Come First Served)\n");
    printf("========================================\n");
    
    for (int i = 0; i < NUM_TASK; i++) {
        base[i].id = i;
        base[i].burst = (rand() % 10) + 1;  // 1-10 unit
    }

    // Simulasi menggunakan single-worker FCFS
    simulate_single_fcfs(base, NUM_TASK);

    // Simulasi menggunakan multi-worker FCFS (5 koki paralel)
    simulate_multiworker_fcfs(base, NUM_TASK);

    printf("========================================\n");
    printf("KESIMPULAN PERBANDINGAN:\n");
    printf("========================================\n");
    printf("Single-Worker:\n");
    printf("  - Semua task dikerjakan secara SEKUENSIAL\n");
    printf("  - Task harus menunggu task sebelumnya selesai\n");
    printf("  - Waiting time dan turnaround time LEBIH TINGGI\n");
    printf("  - Total waktu eksekusi = jumlah semua burst time\n");
    printf("\n");
    printf("Multi-Worker (5 Koki Paralel):\n");
    printf("  - Task dikerjakan secara PARALEL oleh 5 koki\n");
    printf("  - Task dapat langsung dikerjakan jika ada koki yang idle\n");
    printf("  - Waiting time dan turnaround time LEBIH RENDAH\n");
    printf("  - Total waktu eksekusi LEBIH CEPAT (tergantung distribusi burst time)\n");
    printf("  - Throughput meningkat karena paralelisasi\n");
    printf("========================================\n");

    return 0;
}
