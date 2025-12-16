# Perintah untuk Menjalankan main_synchronized.c di VirtualBox (Linux)

## Langkah-langkah:

### 1. Masuk ke direktori proyek
```bash
cd "D:\Semester 7\Sistem Operasi\gofood-multithreading"
```

Atau jika di Linux/VirtualBox, sesuaikan path:
```bash
cd ~/gofood-multithreading
# atau
cd /path/to/gofood-multithreading
```

### 2. Compile program
```bash
gcc -o foodgo_sync main_synchronized.c -Wall -pthread
```

**Penjelasan flag:**
- `-o foodgo_sync` : nama output executable
- `-Wall` : tampilkan semua warning
- `-pthread` : **WAJIB** untuk library pthread

### 3. Jalankan program
```bash
./foodgo_sync
```

---

## Perintah Lengkap (Copy-Paste):

```bash
# Compile
gcc -o foodgo_sync main_synchronized.c -Wall -pthread

# Run
./foodgo_sync
```

---

## Atau menggunakan Makefile (jika tersedia):

```bash
# Build synchronized version
make foodgo_sync

# Run
./foodgo_sync
```

Atau langsung:
```bash
make run-sync
```

---

## Troubleshooting:

### Error: "pthread.h: No such file or directory"
**Solusi:** Install development tools
```bash
# Ubuntu/Debian:
sudo apt-get update
sudo apt-get install build-essential

# CentOS/RHEL:
sudo yum groupinstall "Development Tools"
```

### Error: "undefined reference to 'pthread_create'"
**Solusi:** Pastikan menggunakan flag `-pthread` (bukan `-lpthread`)
```bash
gcc -o foodgo_sync main_synchronized.c -Wall -pthread
```

### Error: Permission denied
**Solusi:** Berikan permission execute
```bash
chmod +x foodgo_sync
./foodgo_sync
```

---

## Expected Output:

Program akan menampilkan:
- Order masuk ke queue
- Payment thread memproses pembayaran
- 5 kitchen thread (koki) mengambil order dan memasak
- Stock thread update stok
- Print thread cetak struk
- Final statistics: Total Orders, Completed, Final Stock

Semua harus konsisten (Completed = 30, Final Stock = 70)

