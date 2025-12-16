#include <stdio.h>
#include <stdlib.h>

#define NUM_TASK 30

typedef struct {
    int id;
    int burst;      // "lama masak" / CPU burst
    int remaining;  // sisa burst (untuk Round Robin)
    int waiting;
    int turnaround;
} Task;

// Utility: cetak ringkasan hasil scheduler
void print_result(const char* name, Task tasks[], int n, int total_time) {
    double avg_wait = 0.0, avg_tat = 0.0;
    printf("\n=== %s ===\n", name);
    printf("ID\tBurst\tWaiting\tTurnaround\n");
    for (int i = 0; i < n; i++) {
        printf("%2d\t%5d\t%7d\t%10d\n",
               tasks[i].id, tasks[i].burst, tasks[i].waiting, tasks[i].turnaround);
        avg_wait += tasks[i].waiting;
        avg_tat  += tasks[i].turnaround;
    }
    avg_wait /= n;
    avg_tat  /= n;

    printf("Total waktu eksekusi: %d unit\n", total_time);
    printf("Rata-rata waiting time   : %.2f\n", avg_wait);
    printf("Rata-rata turnaround time: %.2f\n\n", avg_tat);
}

// FCFS: urutan sesuai antrian (mirip pesanan GoFood masuk)
void simulate_fcfs(Task src[], int n) {
    Task tasks[NUM_TASK];
    for (int i = 0; i < n; i++) tasks[i] = src[i];

    int time = 0;
    printf("Gantt Chart FCFS: ");
    for (int i = 0; i < n; i++) {
        tasks[i].waiting = time;
        time += tasks[i].burst;
        tasks[i].turnaround = time;
        printf("| T%d ", tasks[i].id);
    }
    printf("|\n");

    print_result("FCFS", tasks, n, time);
}

// Round Robin: time-sharing dengan quantum (mirip 5 chef paralel)
void simulate_rr(Task src[], int n, int quantum) {
    Task tasks[NUM_TASK];
    for (int i = 0; i < n; i++) {
        tasks[i] = src[i];
        tasks[i].remaining = src[i].burst;
    }

    int time = 0;
    int completed = 0;
    int queue[NUM_TASK];
    int front = 0, rear = 0;
    int in_queue[NUM_TASK] = {0};
    int completion_time[NUM_TASK] = {0};

    // Masukkan semua task ke queue
    for (int i = 0; i < n; i++) {
        queue[rear++] = i;
        in_queue[i] = 1;
    }

    printf("Gantt Chart RR (quantum=%d): ", quantum);
    
    while (completed < n) {
        if (front == rear) break;
        
        int idx = queue[front++];
        in_queue[idx] = 0;

        if (tasks[idx].remaining > 0) {
            int exec_time = (tasks[idx].remaining > quantum) ? quantum : tasks[idx].remaining;
            
            printf("| T%d(%d) ", tasks[idx].id, exec_time);
            
            time += exec_time;
            tasks[idx].remaining -= exec_time;

            if (tasks[idx].remaining == 0) {
                completed++;
                completion_time[idx] = time;
                tasks[idx].turnaround = time;
                tasks[idx].waiting = tasks[idx].turnaround - tasks[idx].burst;
            } else {
                // Masukkan kembali ke queue
                queue[rear++] = idx;
                in_queue[idx] = 1;
            }
        }
    }
    printf("|\n");

    print_result("Round Robin", tasks, n, time);
}

int main() {
    Task base[NUM_TASK];

    // Contoh: 30 "order" dengan burst time acak 1–10 unit
    for (int i = 0; i < NUM_TASK; i++) {
        base[i].id = i;
        base[i].burst = (rand() % 10) + 1;
        base[i].remaining = base[i].burst;
        base[i].waiting = 0;
        base[i].turnaround = 0;
    }

    printf("========================================\n");
    printf("Simulasi CPU Scheduler untuk %d task (GoFood orders)\n", NUM_TASK);
    printf("========================================\n");

    // Test FCFS
    simulate_fcfs(base, NUM_TASK);

    // Test Round Robin dengan quantum 2
    simulate_rr(base, NUM_TASK, 2);

    // Test Round Robin dengan quantum 4
    simulate_rr(base, NUM_TASK, 4);

    printf("========================================\n");
    printf("KESIMPULAN:\n");
    printf("- FCFS: Simple tapi waiting time tinggi\n");
    printf("- Round Robin: Lebih fair, response time lebih baik\n");
    printf("- Quantum kecil: Overhead tinggi, tapi responsive\n");
    printf("- Quantum besar: Mendekati FCFS\n");
    printf("========================================\n");

    return 0;
}


