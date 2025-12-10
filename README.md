# Go-Food Multithreading System
## Operating Systems Final Project - Semester Ganjil 2025/2026

### 📋 Deskripsi Proyek
Sistem multithreading untuk simulasi pemrosesan order Go-Food yang mendemonstrasikan konsep-konsep Operating Systems seperti race condition, synchronization, memory mapping, dan CPU scheduling.

---

## 👥 Tim Pengembang

### Grup A - Fase Coding
- **Krisopras Eben Haezer** - GitHub & Kerangka Kerja System Call (Memory Mapping + Race Condition)
- **Bryan Sean Abner Manullang** - Synchronization + Security Protection
- **Nathanael Sean Lim** - Scheduler + I/O Management

### Grup B - Fase Dokumentasi/Presentasi
- **Cathleen Claudia** - Latar Belakang, Pendahuluan, Deskripsi Sistem, Flowchart, Pengujian
- **Antony Markus** - Presentasi + Review Keseluruhan

---

## 🎯 Latar Belakang Masalah

### Permasalahan Real
Dalam sistem Go-Food, terdapat beberapa tantangan operasional:
1. **Volume Order Tinggi**: Ribuan order masuk secara bersamaan dari berbagai customer
2. **Proses Sequential**: Order harus melalui tahapan: Penerimaan → Payment → Kitchen → Stock Update
3. **Shared Resources**: Multiple threads mengakses data yang sama (order list, stock inventory, revenue)
4. **Bottleneck**: Tanpa paralelisme, setiap order harus menunggu order sebelumnya selesai sepenuhnya

### Mengapa Membutuhkan Multithreading?
- **Paralelisme**: Memproses multiple orders secara bersamaan
- **Efisiensi**: Mengurangi waiting time dan meningkatkan throughput
- **Resource Utilization**: Memanfaatkan multi-core CPU secara optimal
- **Responsiveness**: Sistem tetap responsif meski load tinggi

### Solusi dengan Teknik OS
1. **Threading**: 4 thread untuk handle berbagai tahapan proses
2. **Synchronization**: Mutex dan semaphore untuk menghindari race condition
3. **Memory Management**: Shared memory dengan mmap untuk inter-thread communication
4. **Scheduling**: CPU scheduler untuk mengatur eksekusi thread

---

## 🏗️ Arsitektur Sistem

### 4 Thread Utama

#### 1. **Order Receiver Thread**
- Menerima order dari customer
- Generate order ID, customer info, food item, price
- Menyimpan ke shared memory
- **Race Condition Point**: `order_count` variable

#### 2. **Payment Processor Thread**
- Memproses pembayaran order
- Validasi dan konfirmasi payment
- Update total revenue
- **Race Condition Point**: `payment_processed`, `total_revenue`
- **Sequential Dependency**: Harus menunggu order diterima terlebih dahulu

#### 3. **Kitchen Processor Thread**
- Memproses cooking order
- Simulasi waktu memasak
- Update status order
- **Race Condition Point**: `kitchen_processed`
- **Sequential Dependency**: Harus menunggu payment confirmed

#### 4. **Stock Manager Thread**
- Update inventory stock
- Monitor low stock warning
- Finalisasi order completion
- **Race Condition Point**: `stock_inventory`
- **Sequential Dependency**: Harus menunggu cooking selesai

---

## 🔧 Fitur Implementasi

### ✅ Race Condition Demonstration
- Mode `--race`: Menjalankan tanpa synchronization untuk menunjukkan race condition
- Hasil: Data inconsistency, incorrect counters, wrong revenue calculation

### ✅ Synchronization
- **Mutex**: Melindungi critical sections
- **Semaphores**: Mengatur sequential dependency antar thread
- **Condition Variables**: Thread coordination
- Mode `--sync`: Menjalankan dengan proper synchronization

### ✅ Memory Mapping
- Menggunakan `mmap()` untuk shared memory allocation
- Visualisasi memory layout
- Analisis memory access patterns per thread

### ✅ Scheduler
- Demonstrasi CPU scheduling untuk thread execution
- Analisis context switching
- Performance comparison

---

## 🚀 Cara Menjalankan

### Prerequisites
```bash
# Ubuntu/WSL
sudo apt-get update
sudo apt-get install build-essential gcc make

# Verify installation
gcc --version
make --version
```

### Build Project
```bash
# Clone repository
git clone <repository-url>
cd gofood-multithreading

# Build
make
```

### Run Program

#### 1. Demonstrasi Race Condition (Tanpa Synchronization)
```bash
make run-race
# atau
./gofood_system
```

#### 2. Mode Synchronized (Dengan Synchronization)
```bash
make run-sync
# atau
./gofood_system --sync
```

#### 3. Jalankan Kedua Mode untuk Perbandingan
```bash
make run-both
```

### Clean Build
```bash
make clean
```

---

## 📊 Output Program

### Memory Mapping Analysis
Program akan menampilkan:
- Alamat memory untuk shared orders
- Alamat memory untuk stock inventory
- Lokasi shared variables (race condition points)

### Execution Log
Setiap thread akan log aktivitasnya:
```
[ORDER] Order #1 received: Customer_1 orders Nasi Goreng (Rp 25000)
[PAYMENT] Order #1 payment confirmed: Rp 25000 (Total: Rp 25000)
[KITCHEN] Order #1 cooking: Nasi Goreng for Customer_1
[STOCK] Order #1 completed. Nasi Goreng stock: 49 remaining
```

### Final Statistics
- Total orders processed
- Total revenue
- Execution time
- Final stock inventory

---

## 📁 Struktur File

```
gofood-multithreading/
├── main.c              # Source code utama
├── Makefile            # Build automation
├── README.md           # Dokumentasi ini
├── docs/               # Dokumentasi tambahan
│   ├── LAPORAN.md      # Laporan lengkap
│   ├── FLOWCHART.png   # Diagram alur sistem
│   └── MEMORY_MAP.png  # Diagram memory mapping
└── screenshots/        # Screenshot hasil eksekusi
    ├── race_condition.png
    └── synchronized.png
```

---

## 🔬 Aspek-Aspek OS yang Diimplementasikan

### 1. Race Condition & Synchronization (45 poin)
- ✅ 4 Thread processing 30+ tasks
- ✅ Race condition demonstration
- ✅ Mutex, semaphores, condition variables
- ✅ Sequential dependency (Thread B & C wait for A, Thread D waits for B & C)

### 2. Memory Mapping (20 poin)
- ✅ `mmap()` system call untuk shared memory
- ✅ Memory layout visualization
- ✅ Race condition memory analysis
- ✅ Thread memory access patterns

### 3. CPU Scheduler (Included in implementation)
- ✅ Thread scheduling demonstration
- ✅ Context switching analysis
- ✅ Performance comparison

### 4. I/O Management (10 poin)
- Analisis polling vs interrupts
- Bottleneck identification
- Real-world I/O handling strategies

### 5. Security & Protection (10 poin)
- Threat model analysis
- Access control matrix
- Potential attacks & mitigations

---

## 📝 TODO List

### Grup A (Coding)
- [x] Setup GitHub repository
- [x] Implementasi kerangka kerja 4 thread
- [x] Implementasi race condition demonstration
- [x] Implementasi memory mapping dengan mmap()
- [ ] Implementasi synchronization (mutex, semaphore) - **@bryanseann**
- [ ] Implementasi CPU scheduler - **@nathanseanlim**
- [ ] Analisis I/O Management - **@nathanseanlim**
- [ ] Analisis Security & Protection - **@bryanseann**

### Grup B (Dokumentasi)
- [ ] Latar belakang & pendahuluan - **@cathleenclau**
- [ ] Flowchart dan diagram alur - **@cathleenclau**
- [ ] Screenshot dan tabel hasil pengujian - **@cathleenclau**
- [ ] Slide presentasi - **Antony**
- [ ] Review keseluruhan - **Antony**

---

## 🧪 Testing & Validation

### Test Cases
1. **Race Condition Test**: Run tanpa sync, verify data inconsistency
2. **Synchronization Test**: Run dengan sync, verify data consistency
3. **Sequential Dependency Test**: Verify thread execution order
4. **Memory Test**: Verify shared memory access
5. **Performance Test**: Compare execution time

### Expected Results
| Metric | Without Sync | With Sync |
|--------|-------------|-----------|
| Data Consistency | ❌ Inconsistent | ✅ Consistent |
| Order Count | ❌ May be wrong | ✅ Correct (30) |
| Revenue | ❌ May be wrong | ✅ Correct |
| Execution Order | ❌ Random | ✅ Sequential |

---

## 📚 Referensi
- Operating Systems: Three Easy Pieces (OSTEP)
- POSIX Threads Programming
- Linux System Programming
- Modern Operating Systems - Tanenbaum

---

## 📅 Timeline
- **Setup & Kerangka**: 10-11 Desember 2024
- **Development**: 11-14 Desember 2024
- **Testing & Documentation**: 14-16 Desember 2024
- **Presentasi**: 17 Desember 2024

---

## 📞 Kontak
Untuk pertanyaan atau diskusi, hubungi anggota tim melalui GitHub issues atau grup WhatsApp.

---

## 📄 License
Educational Project - Operating Systems Course

---

**GBU ALL! 🙏**
