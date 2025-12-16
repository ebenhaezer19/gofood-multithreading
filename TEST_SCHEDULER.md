# Testing Guide untuk main_scheduler.c

## Cara Compile dan Run

### Di Linux/WSL:
```bash
gcc -o scheduler main_scheduler.c -Wall
./scheduler
```

### Di Windows (MinGW):
```bash
gcc -o scheduler.exe main_scheduler.c -Wall
scheduler.exe
```

### Di Windows (Visual Studio):
```bash
cl main_scheduler.c /Fe:scheduler.exe
scheduler.exe
```

## Expected Output

Program akan menampilkan:

1. **Konfigurasi**: Jumlah task (30), jumlah worker (5), algoritma (FCFS)

2. **Gantt Chart Single Worker (SEBELUM)**:
   - Menampilkan urutan task yang dikerjakan secara sekuensial
   - Setiap task harus menunggu task sebelumnya selesai

3. **Gantt Chart Multi-Worker FCFS (SESUDAH)**:
   - Menampilkan 5 koki bekerja paralel
   - Task didistribusikan ke koki yang idle
   - Beberapa time unit pertama ditampilkan sebagai contoh

4. **Tabel Perbandingan**:
   - Waiting time per task
   - Turnaround time per task
   - Rata-rata waiting time
   - Rata-rata turnaround time
   - Total waktu eksekusi

5. **Kesimpulan**:
   - Perbandingan sebelum vs sesudah multi-worker

## Yang Harus Dicek Saat Testing

### ✅ Validasi Output:

1. **Single Worker FCFS**:
   - Total waktu = jumlah semua burst time
   - Waiting time task ke-i = sum burst time task 0 sampai i-1
   - Turnaround time = waiting time + burst time

2. **Multi-Worker FCFS**:
   - Total waktu < total waktu single worker (kecuali edge case)
   - Rata-rata waiting time < rata-rata single worker
   - Rata-rata turnaround time < rata-rata single worker
   - Semua 30 task harus selesai (completed = 30)

3. **Gantt Chart**:
   - Tidak ada koki yang mengerjakan 2 task bersamaan di waktu yang sama
   - Task diambil dari queue sesuai urutan FCFS
   - Setiap task hanya dikerjakan oleh 1 koki

### ⚠️ Warning yang Boleh Diabaikan:
- Jika ada warning tentang unused variable (kecuali error)
- Output Gantt Chart mungkin terpotong (hanya menampilkan beberapa time unit pertama)

### ❌ Error yang Harus Diperbaiki:
- Segmentation fault
- Infinite loop (program tidak selesai)
- Task tidak selesai semua (completed < 30)
- Perhitungan waiting/turnaround time salah

## Contoh Output yang Benar

```
========================================
Simulasi CPU Scheduler Multi-Worker FCFS
========================================
Konfigurasi:
- Jumlah Task/Order: 30
- Jumlah Worker/Koki: 5
- Algoritma: FCFS (First Come First Served)
========================================

Gantt Chart FCFS Single Worker (SEBELUM Multi-Worker):
Time | Worker
-----+-------
   0 | T 0
   5 | T 1
  12 | T 2
...
[lanjutan]

=== FCFS Single Worker (SEBELUM) ===
ID  Burst  Waiting  Turnaround
 0      5        0          5
 1      7        5         12
...

Total waktu eksekusi: [jumlah]
Rata-rata waiting time  : [angka]
Rata-rata turnaround time: [angka]

Gantt Chart Multi-Worker FCFS (5 Koki):
Time | Koki1  Koki2  Koki3  Koki4  Koki5
-----+-----------------------------------
   0 | T 0    T 1    T 2    T 3    T 4
   1 | T 0    T 1    T 2    T 3    T 4
...
[lanjutan]

=== Multi-Worker FCFS ===
[table dengan waiting/turnaround time]

KESIMPULAN PERBANDINGAN:
[perbandingan sebelum vs sesudah]
```

## Troubleshooting

### Program tidak compile:
- Pastikan gcc/compiler terinstall
- Pastikan tidak ada syntax error di kode

### Program hang/tidak selesai:
- Cek apakah ada infinite loop (time > 1000 akan break)
- Pastikan semua task bisa diambil dari queue

### Output tidak sesuai:
- Cek seed random (srand(42)) untuk konsistensi
- Pastikan perhitungan waiting/turnaround time benar

