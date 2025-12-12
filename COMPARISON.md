# Perbandingan Race Condition vs Synchronized

## 📊 Overview

Project ini memiliki **2 versi** untuk mendemonstrasikan perbedaan antara sistem **TANPA** dan **DENGAN** synchronization.

---

## 🔴 Version 1: main_race_condition.c

### Karakteristik
- ❌ **TANPA** mutex/semaphore
- ❌ **TANPA** condition variables
- ❌ **TANPA** thread coordination
- ✅ Mendemonstrasikan **RACE CONDITION**

### Race Condition Points

#### 1. **completed counter**
```c
// RACE CONDITION!
completed++;  // Multiple threads increment tanpa lock
```

**Masalah:**
- Thread A: read `completed = 5`
- Thread B: read `completed = 5` (nilai sama!)
- Thread A: write `completed = 6`
- Thread B: write `completed = 6` (overwrite!)
- **Result**: Lost update! Seharusnya 7, tapi jadi 6

#### 2. **total_revenue**
```c
// RACE CONDITION!
total_revenue += amount;  // Non-atomic operation
```

**Masalah:**
- Thread A: read `total_revenue = 100000`
- Thread A: calculate `100000 + 25000 = 125000`
- Thread B: read `total_revenue = 100000` (belum di-update!)
- Thread A: write `total_revenue = 125000`
- Thread B: calculate `100000 + 30000 = 130000`
- Thread B: write `total_revenue = 130000` (overwrite!)
- **Result**: Lost Rp 25,000!

#### 3. **stock counter**
```c
// RACE CONDITION!
stock -= 1;  // Multiple threads decrement tanpa lock
```

**Masalah:**
- Similar dengan completed counter
- Stock calculation jadi tidak akurat

#### 4. **order_queue (enqueue/dequeue)**
```c
// RACE CONDITION!
order_queue[rear++] = orderID;  // rear++ tidak atomic
int x = order_queue[front++];   // front++ tidak atomic
```

**Masalah:**
- Multiple threads bisa dapat order yang sama
- Order bisa hilang
- Queue corruption

### Expected Results (SALAH!)
```
Total Orders Processed: 27-29  ❌ (seharusnya 30)
Total Revenue: Rp 950000       ❌ (tidak konsisten)
Final Stock: 72                ❌ (seharusnya 70)
```

---

## 🟢 Version 2: main_synchronized.c

### Karakteristik
- ✅ **DENGAN** mutex untuk protect shared resources
- ✅ **DENGAN** condition variables untuk coordination
- ✅ **DENGAN** sequential dependency
- ✅ Data **CONSISTENCY** terjamin

### Synchronization Mechanisms

#### 1. **Mutex Protection**
```c
// PROTECTED dengan mutex
pthread_mutex_lock(&mutex_queue);
order_queue[rear++] = orderID;
pthread_mutex_unlock(&mutex_queue);
```

**Benefit:**
- Hanya 1 thread yang bisa akses queue pada satu waktu
- No lost updates
- Data consistency

#### 2. **Condition Variables**
```c
// Thread coordination
pthread_mutex_lock(&mutex_payment);
payment_status[i] = 1;
pthread_cond_broadcast(&cond_payment);  // Signal payment OK
pthread_mutex_unlock(&mutex_payment);

// Kitchen thread wait
pthread_mutex_lock(&mutex_payment);
while (payment_status[orderID] == 0)
    pthread_cond_wait(&cond_payment, &mutex_payment);
pthread_mutex_unlock(&mutex_payment);
```

**Benefit:**
- Kitchen tidak proses order sebelum payment OK
- Sequential dependency terjaga
- Efficient waiting (tidak busy-wait)

#### 3. **Protected Critical Sections**
```c
// Revenue update - PROTECTED
pthread_mutex_lock(&mutex_payment);
total_revenue += amount;
pthread_mutex_unlock(&mutex_payment);

// Stock update - PROTECTED
pthread_mutex_lock(&mutex_stock);
stock -= 1;
pthread_mutex_unlock(&mutex_stock);

// Completed counter - PROTECTED
pthread_mutex_lock(&mutex_payment);
completed++;
pthread_mutex_unlock(&mutex_payment);
```

**Benefit:**
- Atomic operations
- No race conditions
- Accurate calculations

### Expected Results (BENAR!)
```
Total Orders Processed: 30     ✅ (correct!)
Total Revenue: Rp 1050000      ✅ (consistent!)
Final Stock: 70                ✅ (correct!)
```

---

## 📈 Comparison Table

| Aspect | Race Condition | Synchronized |
|--------|---------------|--------------|
| **Mutex** | ❌ Tidak ada | ✅ Ada |
| **Condition Variables** | ❌ Tidak ada | ✅ Ada |
| **Data Consistency** | ❌ Tidak konsisten | ✅ Konsisten |
| **Order Count** | ❌ 27-29 | ✅ 30 |
| **Revenue Accuracy** | ❌ Tidak akurat | ✅ Akurat |
| **Stock Calculation** | ❌ Salah | ✅ Benar |
| **Sequential Dependency** | ❌ Tidak terjaga | ✅ Terjaga |
| **Performance** | ⚡ Faster (tapi salah!) | 🐢 Slower (tapi benar!) |

---

## 🎯 Key Takeaways

### Race Condition Version
1. **Faster** tapi **SALAH**
2. Mendemonstrasikan **bahaya** concurrent access tanpa protection
3. **Lost updates** terjadi
4. **Data corruption** mungkin terjadi
5. **Tidak production-ready**

### Synchronized Version
1. **Slower** tapi **BENAR**
2. Mendemonstrasikan **pentingnya** synchronization
3. **Data integrity** terjaga
4. **Predictable behavior**
5. **Production-ready**

---

## 💡 Kesimpulan

> **"Correctness > Performance"**

Dalam sistem real-world seperti Go-Food:
- Lost revenue = **Kerugian finansial**
- Stock inconsistency = **Overselling** atau **Underselling**
- Order count salah = **Customer complaints**

**Trade-off** sedikit performance overhead untuk synchronization adalah **acceptable** dan **necessary** untuk menjaga:
- ✅ Data integrity
- ✅ Business logic correctness
- ✅ Customer satisfaction
- ✅ Financial accuracy

---

## 🧪 Testing

### Test Race Condition
```bash
# Run 5 kali dan lihat hasil berbeda-beda
for i in {1..5}; do
    echo "Run $i:"
    ./gofood_race | grep "Total Orders"
done
```

**Expected**: Hasil **BERBEDA** setiap run (race condition!)

### Test Synchronized
```bash
# Run 5 kali dan lihat hasil sama
for i in {1..5}; do
    echo "Run $i:"
    ./gofood_sync | grep "Total Orders"
done
```

**Expected**: Hasil **SAMA** setiap run (consistent!)

---

## 📚 Referensi

- **Race Condition**: Multiple threads accessing shared data without synchronization
- **Mutex**: Mutual Exclusion - only one thread can hold the lock
- **Condition Variable**: Thread signaling mechanism
- **Critical Section**: Code that accesses shared resources
- **Atomic Operation**: Operation that completes without interruption

---

**Dibuat untuk**: Operating Systems Final Project  
**Semester**: Ganjil 2025/2026  
**Tim**: Grup A & B
