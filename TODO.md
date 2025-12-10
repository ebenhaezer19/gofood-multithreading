# TODO List - Go-Food Multithreading Project

## 📅 Timeline
- **Setup & Kerangka**: 10-11 Desember 2024 ✅
- **Development**: 11-14 Desember 2024
- **Testing & Documentation**: 14-16 Desember 2024
- **Presentasi**: 17 Desember 2024

---

## 👥 Grup A - Fase Coding

### 🔧 [Nama Anda] - GitHub & Kerangka Kerja + Memory Mapping + Race Condition

#### ✅ Completed
- [x] Setup GitHub repository
- [x] Buat kerangka kerja 4 thread system
- [x] Implementasi basic thread structure
- [x] Implementasi race condition demonstration
- [x] Implementasi memory mapping dengan mmap()
- [x] Buat README.md
- [x] Buat Makefile
- [x] Buat dokumentasi LAPORAN.md
- [x] Buat SETUP_GUIDE.md
- [x] Buat GITHUB_SETUP.md

#### 📝 In Progress
- [ ] Test race condition demonstration
- [ ] Verify memory mapping output
- [ ] Ambil screenshot hasil eksekusi

#### ⏳ TODO
- [ ] Review code dari @bryanseann (synchronization)
- [ ] Review code dari @nathanseanlim (scheduler)
- [ ] Integration testing semua components
- [ ] Final code review

---

### 🔐 @bryanseann - Synchronization + Security Protection

#### ⏳ TODO - Synchronization (Priority: HIGH)
- [ ] **Verify mutex implementation** di `main.c`
  - [ ] Test mutex untuk `order_mutex`
  - [ ] Test mutex untuk `payment_mutex`
  - [ ] Test mutex untuk `kitchen_mutex`
  - [ ] Test mutex untuk `stock_mutex`
  - [ ] Test mutex untuk `revenue_mutex`

- [ ] **Verify semaphore implementation**
  - [ ] Test `order_received_sem`
  - [ ] Test `payment_confirmed_sem`
  - [ ] Test `cooking_done_sem`

- [ ] **Verify condition variables**
  - [ ] Test `order_ready_cond`
  - [ ] Test `payment_ready_cond`
  - [ ] Test `kitchen_ready_cond`

- [ ] **Testing**
  - [ ] Run program dengan `--sync` flag
  - [ ] Verify data consistency (order count = 30)
  - [ ] Verify revenue accuracy
  - [ ] Verify sequential dependency (order → payment → kitchen → stock)
  - [ ] Compare performance: sync vs unsync

#### ⏳ TODO - Security & Protection (Priority: MEDIUM)
- [ ] **Analisis Threat Model** (di LAPORAN.md)
  - [ ] Identifikasi potential attacks
  - [ ] Race condition exploitation scenarios
  - [ ] Memory corruption threats
  - [ ] DoS attack vectors
  - [ ] Payment bypass scenarios

- [ ] **Access Control Matrix**
  - [ ] Define access rights per thread
  - [ ] Implement principle of least privilege
  - [ ] Document access matrix di LAPORAN.md

- [ ] **Security Mechanisms**
  - [ ] Input validation (buffer overflow prevention)
  - [ ] Rate limiting (DoS prevention)
  - [ ] Stock validation (overselling prevention)
  - [ ] Payment verification

- [ ] **Documentation**
  - [ ] Lengkapi BAB V di LAPORAN.md
  - [ ] Add security best practices
  - [ ] Add audit logging recommendations

#### 📅 Deadline
- **Synchronization**: 12 Desember 2024
- **Security Analysis**: 14 Desember 2024

---

### ⚙️ @nathanseanlim - Scheduler + I/O Management

#### ⏳ TODO - CPU Scheduler (Priority: HIGH)
- [ ] **Analisis Thread Scheduling**
  - [ ] Study thread scheduling behavior
  - [ ] Analyze context switching
  - [ ] Measure context switch overhead
  - [ ] Document scheduling patterns

- [ ] **Performance Analysis**
  - [ ] Compare execution time: sync vs unsync
  - [ ] Measure CPU utilization per thread
  - [ ] Analyze thread interleaving patterns
  - [ ] Document performance metrics

- [ ] **Scheduler Implementation** (Optional - jika ada waktu)
  - [ ] Implement custom thread priority
  - [ ] Test different scheduling policies
  - [ ] Compare FIFO vs Round-Robin

- [ ] **Documentation**
  - [ ] Lengkapi section 2.5 di LAPORAN.md (CPU Scheduler Analysis)
  - [ ] Add performance comparison tables
  - [ ] Add scheduling diagrams

#### ⏳ TODO - I/O Management (Priority: MEDIUM)
- [ ] **Analisis I/O Operations**
  - [ ] Identifikasi jenis-jenis I/O dalam sistem
  - [ ] Analyze I/O characteristics (frequency, latency, throughput)
  - [ ] Document I/O patterns

- [ ] **I/O Handling Methods**
  - [ ] Explain polling approach
  - [ ] Explain interrupt-driven I/O
  - [ ] Explain DMA
  - [ ] Compare pros/cons

- [ ] **Bottleneck Analysis**
  - [ ] Identify I/O bottlenecks
  - [ ] Database I/O bottleneck
  - [ ] Network I/O bottleneck
  - [ ] Disk I/O bottleneck
  - [ ] Propose solutions

- [ ] **OS Solutions**
  - [ ] Research Linux I/O scheduler (CFQ, Deadline, NOOP)
  - [ ] Research Asynchronous I/O (AIO)
  - [ ] Research Memory-Mapped I/O
  - [ ] Research I/O Buffering

- [ ] **Documentation**
  - [ ] Lengkapi BAB IV di LAPORAN.md
  - [ ] Add I/O bottleneck analysis table
  - [ ] Add recommendations

#### 📅 Deadline
- **Scheduler Analysis**: 13 Desember 2024
- **I/O Management**: 14 Desember 2024

---

## 👥 Grup B - Fase Dokumentasi/Presentasi

### 📄 @cathleenclau - Dokumentasi & Pengujian

#### ⏳ TODO - Latar Belakang & Pendahuluan (Priority: HIGH)
- [ ] **Lengkapi BAB I di LAPORAN.md**
  - [ ] Review dan polish latar belakang yang sudah ada
  - [ ] Tambahkan detail spesifik Go-Food
  - [ ] Tambahkan statistik/data real (jika ada)
  - [ ] Pastikan flow logis dan mudah dipahami

#### ⏳ TODO - Deskripsi Sistem (Priority: HIGH)
- [ ] **Lengkapi BAB II di LAPORAN.md**
  - [ ] Review deskripsi 4 thread
  - [ ] Tambahkan detail implementasi
  - [ ] Pastikan penjelasan mudah dipahami

#### ⏳ TODO - Flowchart & Diagram (Priority: HIGH)
- [ ] **Buat System Flowchart**
  - [ ] Flowchart keseluruhan sistem
  - [ ] Flowchart per thread
  - [ ] Sequential dependency diagram
  - [ ] Save as `docs/FLOWCHART.png`

- [ ] **Buat Memory Mapping Diagram**
  - [ ] Diagram memory layout
  - [ ] Diagram shared memory access
  - [ ] Race condition points visualization
  - [ ] Save as `docs/MEMORY_MAP.png`

- [ ] **Tools yang bisa digunakan**:
  - Draw.io (https://app.diagrams.net/)
  - Lucidchart
  - Microsoft Visio
  - Mermaid (markdown diagram)

#### ⏳ TODO - Pengujian (Priority: HIGH)
- [ ] **Jalankan Program**
  - [ ] Setup WSL/Ubuntu
  - [ ] Compile program: `make`
  - [ ] Run race condition mode: `make run-race`
  - [ ] Run synchronized mode: `make run-sync`
  - [ ] Run comparison: `make run-both`

- [ ] **Ambil Screenshots**
  - [ ] Screenshot race condition output
  - [ ] Screenshot synchronized output
  - [ ] Screenshot memory mapping output
  - [ ] Screenshot final statistics
  - [ ] Save ke folder `screenshots/`

- [ ] **Buat Tabel Hasil Pengujian**
  - [ ] Tabel perbandingan race vs sync
  - [ ] Tabel performance metrics
  - [ ] Tabel data consistency
  - [ ] Add ke LAPORAN.md

- [ ] **Testing Scenarios**
  - [ ] Test 1: Race condition verification (run 5x)
  - [ ] Test 2: Synchronization verification (run 5x)
  - [ ] Test 3: Performance comparison
  - [ ] Test 4: Memory mapping verification
  - [ ] Document results

#### 📅 Deadline
- **Latar Belakang**: 12 Desember 2024
- **Flowchart**: 13 Desember 2024
- **Pengujian**: 14 Desember 2024

---

### 🎤 Antony - Presentasi & Review

#### ⏳ TODO - Presentasi (Priority: HIGH)
- [ ] **Buat Slide Presentasi**
  - [ ] Slide 1: Title & Team
  - [ ] Slide 2-3: Latar Belakang Masalah
  - [ ] Slide 4-5: Arsitektur Sistem (4 Thread)
  - [ ] Slide 6-7: Race Condition Demonstration
  - [ ] Slide 8-9: Synchronization Solution
  - [ ] Slide 10-11: Memory Mapping
  - [ ] Slide 12: CPU Scheduler
  - [ ] Slide 13: I/O Management
  - [ ] Slide 14: Security & Protection
  - [ ] Slide 15: Demo Video/Screenshots
  - [ ] Slide 16: Hasil Pengujian
  - [ ] Slide 17: Kesimpulan
  - [ ] Slide 18: Q&A

- [ ] **Persiapan Demo**
  - [ ] Prepare demo script
  - [ ] Test demo di laptop presentasi
  - [ ] Backup: video recording demo
  - [ ] Prepare untuk live coding (jika diminta)

- [ ] **Persiapan Q&A**
  - [ ] Anticipate possible questions
  - [ ] Prepare answers untuk common questions:
    - "Apa perbedaan mutex dan semaphore?"
    - "Bagaimana cara handle deadlock?"
    - "Kenapa pakai C bukan Python?"
    - "Apa trade-off synchronization?"
    - "Bagaimana scale ke production?"

#### ⏳ TODO - Review Keseluruhan (Priority: HIGH)
- [ ] **Code Review**
  - [ ] Review `main.c` - check for bugs
  - [ ] Review synchronization implementation
  - [ ] Review memory management
  - [ ] Test compilation di WSL/Ubuntu
  - [ ] Test execution (both modes)

- [ ] **Documentation Review**
  - [ ] Review README.md - check completeness
  - [ ] Review LAPORAN.md - check all sections
  - [ ] Review SETUP_GUIDE.md - verify steps
  - [ ] Check for typos and grammar
  - [ ] Ensure consistent formatting

- [ ] **Testing Review**
  - [ ] Verify all test cases passed
  - [ ] Verify screenshots quality
  - [ ] Verify tables and diagrams
  - [ ] Ensure reproducibility

- [ ] **Final Checks**
  - [ ] All files uploaded to GitHub
  - [ ] Repository accessible to team
  - [ ] README looks good on GitHub
  - [ ] All links working
  - [ ] No sensitive data committed

#### 📅 Deadline
- **Slide Presentasi**: 15 Desember 2024
- **Review Keseluruhan**: 16 Desember 2024
- **Rehearsal**: 16 Desember 2024 (malam)

---

## 👥 Grup C - Fase Berdoa

### 🙏 Everyone
- [ ] Berdoa sebelum mulai coding
- [ ] Berdoa sebelum testing
- [ ] Berdoa sebelum presentasi
- [ ] **GBU ALL!** ✨

---

## 📋 Integration Checklist

### Before Presentasi (16 Desember 2024)
- [ ] All code merged to main branch
- [ ] All documentation completed
- [ ] All screenshots uploaded
- [ ] All diagrams created
- [ ] Presentation slides ready
- [ ] Demo tested and working
- [ ] GitHub repository clean and organized
- [ ] README.md updated with final info
- [ ] Team rehearsal done

### Presentasi Day (17 Desember 2024)
- [ ] Laptop charged
- [ ] Backup slides di USB
- [ ] Backup demo video
- [ ] GitHub repository accessible
- [ ] Team ready
- [ ] Confident and prepared
- [ ] **GOOD LUCK!** 🍀

---

## 📞 Communication

### Daily Standup (Optional)
- **Time**: 20:00 WIB
- **Duration**: 15 minutes
- **Format**:
  - What did you do today?
  - What will you do tomorrow?
  - Any blockers?

### Group Chat
- Update progress di grup WhatsApp
- Share screenshots/hasil testing
- Ask questions jika stuck
- Help each other!

---

## 🆘 Jika Ada Masalah

### Technical Issues
1. Check SETUP_GUIDE.md
2. Check GITHUB_SETUP.md
3. Google error message
4. Ask di grup chat
5. Ask ChatGPT/Claude

### Coordination Issues
1. Communicate di grup
2. Adjust timeline if needed
3. Help each other
4. Stay positive!

---

## 🎯 Success Criteria

### Minimum Requirements (Must Have)
- [x] 4 threads implemented
- [x] 30+ tasks processed
- [x] Race condition demonstrated
- [x] Synchronization implemented
- [x] Memory mapping with mmap()
- [ ] Complete documentation
- [ ] Working demo
- [ ] Presentation ready

### Nice to Have (Optional)
- [ ] Custom CPU scheduler
- [ ] Advanced I/O implementation
- [ ] Security mechanisms implemented
- [ ] Performance profiling
- [ ] Video demo
- [ ] Beautiful slides

---

## 📊 Progress Tracking

### Overall Progress: 60%

- **Coding**: 70% ✅
  - Kerangka kerja: 100% ✅
  - Race condition: 100% ✅
  - Memory mapping: 100% ✅
  - Synchronization: 80% 🔄 (@bryanseann)
  - Scheduler: 50% 🔄 (@nathanseanlim)
  - I/O Management: 40% 🔄 (@nathanseanlim)
  - Security: 30% 🔄 (@bryanseann)

- **Documentation**: 50% 🔄
  - README: 100% ✅
  - LAPORAN: 60% 🔄
  - Flowchart: 0% ⏳ (@cathleenclau)
  - Screenshots: 0% ⏳ (@cathleenclau)

- **Presentation**: 0% ⏳ (Antony)

---

**Last Updated**: 10 Desember 2024, 23:45 WIB

**Next Update**: 11 Desember 2024, 20:00 WIB

---

**Let's do this! 💪**
