# Presentation Notes - Go-Food Multithreading System

## 🎯 Key Points untuk Presentasi (17 Desember 2024)

### Opening (1 menit)
- **Perkenalan Tim**: Sebutkan nama kelompok dan anggota
- **Topik**: Sistem Multithreading Go-Food
- **Tujuan**: Mendemonstrasikan konsep OS dalam permasalahan real

---

## 📋 Main Content

### 1. Latar Belakang (2-3 menit)

**Key Points**:
- Go-Food memproses ribuan order per menit
- Permasalahan: Volume tinggi, shared resources, sequential dependencies
- Tanpa multithreading: Bottleneck, slow, inefficient
- Dengan multithreading: Parallel processing, faster, efficient

**Talking Points**:
> "Bayangkan saat jam makan siang, Go-Food menerima ribuan order secara bersamaan. Jika diproses satu per satu, customer harus menunggu sangat lama. Dengan multithreading, kita bisa memproses multiple orders secara parallel, meningkatkan throughput dan mengurangi waiting time."

---

### 2. Arsitektur Sistem (3-4 menit)

**Key Points**:
- **4 Thread**:
  1. Order Receiver - Menerima order dari customer
  2. Payment Processor - Memproses pembayaran
  3. Kitchen Processor - Memproses cooking
  4. Stock Manager - Update inventory

- **Shared Resources**:
  - `shared_orders[]` - Order database
  - `order_count` - Counter
  - `total_revenue` - Revenue accumulator
  - `stock_inventory[]` - Stock data

**Diagram**: Tunjukkan flowchart

**Talking Points**:
> "Sistem kami terdiri dari 4 thread yang bekerja secara concurrent. Setiap thread memiliki tanggung jawab spesifik, namun mereka semua mengakses shared resources yang sama. Ini adalah sumber dari race condition."

---

### 3. Race Condition Demonstration (4-5 menit)

**Key Points**:
- **Apa itu Race Condition?**
  - Multiple threads akses shared data bersamaan
  - Tanpa koordinasi → data corruption
  - Non-atomic operations → lost updates

- **Contoh Konkret**:
  ```
  Thread A: read order_count = 5
  Thread B: read order_count = 5  ← Same value!
  Thread A: write order_count = 6
  Thread B: write order_count = 6  ← Overwrite! Should be 7
  ```

- **Hasil Demo**:
  - Order count ≠ 30 (expected 30)
  - Revenue tidak akurat
  - Data inconsistent

**Demo**: Tunjukkan screenshot race condition

**Talking Points**:
> "Ketika kita jalankan program tanpa synchronization, kita lihat bahwa order count tidak mencapai 30, revenue tidak akurat, dan data tidak konsisten. Ini adalah race condition - multiple threads berkompetisi pada shared resource tanpa koordinasi."

---

### 4. Synchronization Solution (4-5 menit)

**Key Points**:
- **Mutex** (Mutual Exclusion):
  - Lock critical section
  - Hanya satu thread bisa akses
  - Prevent race condition

- **Semaphore**:
  - Sequential dependency
  - Thread B wait untuk Thread A
  - Ensure correct order

- **Condition Variables**:
  - Thread signaling
  - Efficient waiting

**Code Example**:
```c
pthread_mutex_lock(&order_mutex);
// CRITICAL SECTION - Protected
order_count++;
pthread_mutex_unlock(&order_mutex);
```

**Hasil**:
- ✅ Order count = 30 (always)
- ✅ Revenue accurate
- ✅ Data consistent

**Demo**: Tunjukkan screenshot synchronized

**Talking Points**:
> "Dengan menggunakan mutex dan semaphore, kita bisa melindungi critical section dan menjaga sequential dependency. Hasilnya, data selalu konsisten, order count selalu 30, dan revenue akurat."

---

### 5. Memory Mapping (2-3 menit)

**Key Points**:
- **mmap() System Call**:
  - Allocate shared memory
  - Efficient inter-thread communication
  - OS-managed memory

- **Memory Layout**:
  - Shared orders: 12,288 bytes
  - Stock inventory: 512 bytes
  - Global counters: 16 bytes

- **Race Condition Points**:
  - `order_count` @ 0x601040
  - `total_revenue` @ 0x60104c
  - `shared_orders[]` @ 0x7f8a4c000000

**Diagram**: Tunjukkan memory map diagram

**Talking Points**:
> "Kita menggunakan mmap() system call untuk allocate shared memory. Semua thread mengakses memory yang sama, dan inilah yang menyebabkan race condition jika tidak di-synchronize."

---

### 6. CPU Scheduler (1-2 menit)

**Key Points**:
- OS scheduler mengatur thread execution
- Context switching overhead
- Synchronization mengurangi unnecessary switches
- Trade-off: Correctness vs Performance

**Metrics**:
- Without sync: ~3.2s
- With sync: ~4.5s (+40%)
- Trade-off acceptable untuk correctness

---

### 7. I/O Management (1-2 menit)

**Key Points**:
- **I/O Bottlenecks**:
  - Database writes: 50ms each
  - Network latency: 200ms
  - Disk logging: 5ms

- **Solutions**:
  - Batch writes (6× faster)
  - Async I/O (15× faster)
  - Buffering (10× faster)

---

### 8. Security & Protection (1-2 menit)

**Key Points**:
- **Threats**:
  - Race condition exploitation
  - Buffer overflow
  - DoS attacks
  - Stock manipulation

- **Protections**:
  - Synchronization (prevent race)
  - Input validation (prevent overflow)
  - Rate limiting (prevent DoS)
  - Access control (least privilege)

---

### 9. Hasil Pengujian (2 menit)

**Tabel Perbandingan**:

| Metric | Without Sync | With Sync |
|--------|--------------|-----------|
| Data Consistency | 70-85% | 100% |
| Order Count | 26-29 | 30 |
| Revenue Accuracy | 75-85% | 100% |
| Execution Time | 3.2s | 4.5s |

**Talking Points**:
> "Dari hasil pengujian, kita lihat bahwa synchronization berhasil meningkatkan data consistency dari 70-85% menjadi 100%. Trade-off-nya adalah execution time yang sedikit lebih lama, namun ini acceptable untuk correctness."

---

### 10. Kesimpulan (1 menit)

**Key Points**:
- ✅ Berhasil implementasi 4 thread processing 30+ tasks
- ✅ Demonstrasi race condition dan solusinya
- ✅ Memory mapping dengan mmap()
- ✅ Analisis scheduler, I/O, dan security
- ✅ Sistem real-world applicable

**Lessons Learned**:
- Concurrency is hard
- Synchronization is essential
- Performance trade-offs acceptable
- OS concepts critical untuk real systems

---

## 🎤 Demo Script

### Live Demo (jika diminta):

```bash
# 1. Show compilation
make clean
make

# 2. Run race condition
./gofood_system

# 3. Run synchronized
./gofood_system --sync

# 4. Compare results
make run-both
```

### Backup: Video Demo
- Prepare video recording (2-3 minutes)
- Show compilation, execution, results
- Highlight key differences

---

## ❓ Anticipated Q&A

### Q1: "Apa perbedaan mutex dan semaphore?"
**A**: Mutex untuk mutual exclusion (protect critical section), semaphore untuk signaling dan counting (sequential dependency).

### Q2: "Bagaimana handle deadlock?"
**A**: Kita avoid deadlock dengan consistent lock ordering dan tidak hold multiple locks simultaneously. Dalam sistem ini, setiap thread hanya lock satu mutex at a time.

### Q3: "Kenapa pakai C bukan Python?"
**A**: Untuk system programming dan OS concepts, C lebih appropriate karena:
- Direct access ke system calls (pthread, mmap)
- Better control over memory
- Lower overhead
- Industry standard untuk OS development

### Q4: "Apa trade-off synchronization?"
**A**: Trade-off utama adalah performance overhead (~40% slower). Namun ini acceptable karena:
- Data correctness lebih penting
- Prevent financial loss (revenue miscalculation)
- Prevent business disruption (stock issues)

### Q5: "Bagaimana scale ke production?"
**A**: Untuk production:
- Add real database (PostgreSQL)
- Add network layer (REST API)
- Add monitoring (Grafana)
- Scale to multiple processes (message queues)
- Add load balancing
- Add caching layer

### Q6: "Apa yang paling challenging?"
**A**: Race condition debugging - subtle, hard to reproduce, timing-dependent. Membutuhkan careful analysis dan systematic testing.

### Q7: "Bagaimana verify correctness?"
**A**: Multiple methods:
- Automated testing (run 100× times)
- Valgrind helgrind (race detection)
- Manual code review
- Stress testing dengan high load

---

## 📊 Presentation Checklist

### Before Presentation:
- [ ] Laptop fully charged
- [ ] Slides ready (PDF backup)
- [ ] Demo tested and working
- [ ] Video backup prepared
- [ ] GitHub repository accessible
- [ ] Code printout (if needed)
- [ ] Team roles assigned
- [ ] Rehearsal done (2×)

### During Presentation:
- [ ] Speak clearly and confidently
- [ ] Make eye contact
- [ ] Engage audience
- [ ] Time management (15-20 min)
- [ ] Handle Q&A professionally

### After Presentation:
- [ ] Thank audience and professor
- [ ] Collect feedback
- [ ] Celebrate! 🎉

---

## 🎯 Success Criteria

### Must Demonstrate:
1. ✅ Race condition (data inconsistency)
2. ✅ Synchronization fix (data consistency)
3. ✅ Memory mapping visualization
4. ✅ Performance comparison
5. ✅ Understanding of OS concepts

### Bonus Points:
- Live demo successful
- Clear explanations
- Good Q&A handling
- Professional presentation
- Team coordination

---

## 💡 Tips

1. **Practice**: Rehearse minimal 2× dengan team
2. **Timing**: Keep to 15-20 minutes
3. **Clarity**: Explain concepts clearly, avoid jargon
4. **Confidence**: You know your project, be confident!
5. **Backup**: Always have backup (video, PDF, etc.)
6. **Team**: Support each other, help if someone stuck
7. **Relax**: Take deep breath, you got this! 💪

---

## 🙏 Final Words

> "This project demonstrates real-world application of Operating Systems concepts. We've shown how multithreading, synchronization, memory management, and other OS techniques solve actual problems in production systems like Go-Food. Thank you for your attention, and we're happy to answer any questions."

---

**Presentation Date**: 17 Desember 2024  
**Duration**: 15-20 minutes  
**Format**: Slides + Demo + Q&A

---

**GOOD LUCK TEAM! GBU ALL! 🙏✨**
