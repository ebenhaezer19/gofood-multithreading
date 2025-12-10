# Setup Guide - Go-Food Multithreading System

## 📋 Prerequisites

### Windows dengan WSL (Ubuntu)

#### 1. Install WSL
```powershell
# Jalankan di PowerShell sebagai Administrator
wsl --install
```

#### 2. Install Ubuntu
```powershell
wsl --install -d Ubuntu
```

#### 3. Setup Ubuntu
```bash
# Update package list
sudo apt-get update
sudo apt-get upgrade -y

# Install build tools
sudo apt-get install -y build-essential gcc make git

# Verify installation
gcc --version
make --version
git --version
```

### VirtualBox dengan Ubuntu

#### 1. Download Ubuntu ISO
- Download dari: https://ubuntu.com/download/desktop
- Pilih Ubuntu 22.04 LTS atau 24.04 LTS

#### 2. Install di VirtualBox
- Buat VM baru dengan minimal:
  - RAM: 2GB
  - Storage: 20GB
  - CPU: 2 cores

#### 3. Install Build Tools
```bash
sudo apt-get update
sudo apt-get install -y build-essential gcc make git
```

---

## 🚀 Installation

### 1. Clone Repository
```bash
# Clone dari GitHub
git clone https://github.com/[username]/gofood-multithreading.git
cd gofood-multithreading
```

### 2. Build Project
```bash
# Compile program
make

# Output:
# gcc -Wall -Wextra -pthread -g -o gofood_system main.c
# Build successful! Executable: gofood_system
```

### 3. Verify Build
```bash
# Check executable
ls -lh gofood_system

# Output:
# -rwxr-xr-x 1 user user 45K Dec 10 23:00 gofood_system
```

---

## 🎯 Running the Program

### Mode 1: Race Condition Demonstration (Tanpa Synchronization)

```bash
# Method 1: Direct execution
./gofood_system

# Method 2: Using Makefile
make run-race
```

**Expected Output**:
```
========================================
   GO-FOOD MULTITHREADING SYSTEM
========================================

MODE: UNSYNCHRONIZED (Race condition demonstration)

========== MEMORY MAPPING ANALYSIS ==========
Shared Order Memory: 0x7f8a4c000000 - 0x7f8a4c003000 (Size: 12288 bytes)
Shared Stock Memory: 0x7f8a4c004000 - 0x7f8a4c004200 (Size: 512 bytes)

Shared Variables (Race Condition Points):
  - order_count: 0x601040
  - payment_processed: 0x601044
  - kitchen_processed: 0x601048
  - total_revenue: 0x60104c
=============================================

Starting threads...

[ORDER RECEIVER] Thread 1 started
[PAYMENT PROCESSOR] Thread 2 started
[KITCHEN] Thread 3 started
[STOCK MANAGER] Thread 4 started
[ORDER] Order #1 received: Customer_1 orders Nasi Goreng (Rp 25000)
[PAYMENT] Order #1 payment confirmed: Rp 25000 (Total: Rp 25000)
[ORDER] Order #2 received: Customer_2 orders Mie Goreng (Rp 30000)
...

========================================
         FINAL STATISTICS
========================================
Total Orders Received: 28          ← RACE CONDITION! Should be 30
Total Payments Processed: 27       ← INCONSISTENT!
Total Kitchen Processed: 26        ← INCONSISTENT!
Total Revenue: Rp 745000          ← WRONG! Inconsistent
Execution Time: 3 seconds

Final Stock Inventory:
  - Nasi Goreng: 42
  - Mie Goreng: 35
  - Ayam Geprek: 26
  - Sate Ayam: 21
  - Bakso: 31
========================================
```

**⚠️ Perhatikan**: 
- Order count tidak mencapai 30
- Revenue tidak akurat
- Data inconsistent

---

### Mode 2: Synchronized (Dengan Synchronization)

```bash
# Method 1: Direct execution
./gofood_system --sync

# Method 2: Using Makefile
make run-sync
```

**Expected Output**:
```
========================================
   GO-FOOD MULTITHREADING SYSTEM
========================================

MODE: SYNCHRONIZED (Race condition handled)

========== MEMORY MAPPING ANALYSIS ==========
Shared Order Memory: 0x7f8a4c000000 - 0x7f8a4c003000 (Size: 12288 bytes)
Shared Stock Memory: 0x7f8a4c004000 - 0x7f8a4c004200 (Size: 512 bytes)

Shared Variables (Race Condition Points):
  - order_count: 0x601040
  - payment_processed: 0x601044
  - kitchen_processed: 0x601048
  - total_revenue: 0x60104c
=============================================

Starting threads...

[ORDER RECEIVER] Thread 1 started
[PAYMENT PROCESSOR] Thread 2 started
[KITCHEN] Thread 3 started
[STOCK MANAGER] Thread 4 started
[ORDER] Order #1 received: Customer_1 orders Nasi Goreng (Rp 25000)
[PAYMENT] Order #1 payment confirmed: Rp 25000 (Total: Rp 25000)
[KITCHEN] Order #1 cooking: Nasi Goreng for Customer_1
[STOCK] Order #1 completed. Nasi Goreng stock: 49 remaining
[ORDER] Order #2 received: Customer_2 orders Mie Goreng (Rp 30000)
[PAYMENT] Order #2 payment confirmed: Rp 30000 (Total: Rp 55000)  ← CORRECT!
...

========================================
         FINAL STATISTICS
========================================
Total Orders Received: 30          ← CORRECT!
Total Payments Processed: 30       ← CORRECT!
Total Kitchen Processed: 30        ← CORRECT!
Total Revenue: Rp 900000          ← CORRECT!
Execution Time: 4 seconds

Final Stock Inventory:
  - Nasi Goreng: 44
  - Mie Goreng: 34
  - Ayam Geprek: 25
  - Sate Ayam: 20
  - Bakso: 30
========================================
```

**✅ Perhatikan**:
- Semua counter correct (30)
- Revenue akurat
- Data consistent
- Sequential order terjaga

---

### Mode 3: Comparison (Jalankan Kedua Mode)

```bash
make run-both
```

Output akan menampilkan kedua mode secara berurutan untuk perbandingan.

---

## 📊 Testing & Validation

### Test 1: Race Condition Verification

```bash
# Run 5 kali tanpa sync
for i in {1..5}; do
    echo "Run $i:"
    ./gofood_system | grep "Total Orders"
done
```

**Expected**: Hasil berbeda-beda setiap run (race condition)

### Test 2: Synchronization Verification

```bash
# Run 5 kali dengan sync
for i in {1..5}; do
    echo "Run $i:"
    ./gofood_system --sync | grep "Total Orders"
done
```

**Expected**: Hasil sama setiap run (consistent)

### Test 3: Performance Comparison

```bash
# Measure execution time
time ./gofood_system
time ./gofood_system --sync
```

**Expected**: Synchronized mode ~40% slower (trade-off untuk correctness)

---

## 🐛 Troubleshooting

### Error: "command not found: make"

**Solution**:
```bash
sudo apt-get install make
```

### Error: "pthread.h: No such file or directory"

**Solution**:
```bash
sudo apt-get install build-essential
```

### Error: "Permission denied: ./gofood_system"

**Solution**:
```bash
chmod +x gofood_system
```

### Error: "mmap failed: Cannot allocate memory"

**Solution**: Increase system memory atau reduce NUM_ORDERS

```c
// Edit main.c
#define NUM_ORDERS 10  // Reduce from 30
```

### Program Hang / Tidak Selesai

**Possible Causes**:
1. Deadlock (jika modify synchronization code)
2. Infinite loop

**Solution**:
```bash
# Kill program
Ctrl+C

# Check for zombie processes
ps aux | grep gofood_system
kill -9 [PID]
```

---

## 📝 Development Workflow

### 1. Make Changes
```bash
# Edit source code
nano main.c
# atau
vim main.c
# atau gunakan VS Code
code main.c
```

### 2. Rebuild
```bash
make clean
make
```

### 3. Test
```bash
make run-sync
```

### 4. Commit Changes
```bash
git add .
git commit -m "Description of changes"
git push origin main
```

---

## 🔧 Advanced Usage

### Debug Mode

```bash
# Compile with debug symbols
gcc -Wall -Wextra -pthread -g -O0 -o gofood_system main.c

# Run with gdb
gdb ./gofood_system

# GDB commands:
(gdb) break main
(gdb) run --sync
(gdb) next
(gdb) print order_count
(gdb) continue
```

### Memory Analysis with Valgrind

```bash
# Install valgrind
sudo apt-get install valgrind

# Check for memory leaks
valgrind --leak-check=full ./gofood_system --sync

# Check for race conditions
valgrind --tool=helgrind ./gofood_system
```

### Performance Profiling

```bash
# Install perf
sudo apt-get install linux-tools-generic

# Profile program
perf record ./gofood_system --sync
perf report
```

---

## 📚 Additional Resources

### Documentation
- `README.md` - Project overview
- `docs/LAPORAN.md` - Full report
- `docs/FLOWCHART.png` - System flowchart
- `docs/MEMORY_MAP.png` - Memory diagram

### Learning Resources
- [POSIX Threads Tutorial](https://computing.llnl.gov/tutorials/pthreads/)
- [Linux System Programming](https://man7.org/linux/man-pages/)
- [Operating Systems: Three Easy Pieces](https://pages.cs.wisc.edu/~remzi/OSTEP/)

---

## 🆘 Getting Help

### GitHub Issues
Jika menemukan bug atau ada pertanyaan, buat issue di GitHub repository.

### Contact Team
- **Grup A (Coding)**: [Email/Discord]
- **Grup B (Dokumentasi)**: [Email/Discord]

---

## ✅ Checklist untuk Presentasi

- [ ] Program berhasil di-compile
- [ ] Mode race condition berjalan dan menunjukkan inconsistency
- [ ] Mode synchronized berjalan dan data consistent
- [ ] Screenshot hasil eksekusi sudah diambil
- [ ] Memory mapping output sudah di-capture
- [ ] Perbandingan hasil sudah di-dokumentasikan
- [ ] Flowchart sudah dibuat
- [ ] Laporan sudah lengkap
- [ ] Slide presentasi sudah siap
- [ ] Demo sudah di-rehearse

---

**Good luck dengan presentasi! 🙏**
