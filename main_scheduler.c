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

    printf("Simulasi CPU Scheduler untuk %d task (GoFood orders)\n", NUM_TASK);

    simulate_fcfs(base, NUM_TASK);
    return 0;
}


