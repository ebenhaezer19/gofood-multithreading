# Screenshots

## 📸 Folder ini untuk menyimpan screenshot hasil eksekusi program

### Screenshots yang diperlukan:

1. **race_condition.png**
   - Screenshot output program tanpa synchronization
   - Menunjukkan data inconsistency
   - Command: `./gofood_system`

2. **synchronized.png**
   - Screenshot output program dengan synchronization
   - Menunjukkan data consistency
   - Command: `./gofood_system --sync`

3. **memory_mapping.png**
   - Screenshot memory mapping analysis
   - Menunjukkan alamat memory shared resources

4. **final_statistics.png**
   - Screenshot final statistics
   - Perbandingan hasil race vs sync

5. **compilation.png**
   - Screenshot proses compilation
   - Command: `make`

### Cara mengambil screenshot:

#### Di WSL/Ubuntu:
```bash
# Install screenshot tool (optional)
sudo apt-get install scrot

# Run program dan capture output
./gofood_system > output.txt
./gofood_system --sync > output_sync.txt

# Atau screenshot terminal window langsung
```

#### Di Windows:
- Gunakan Snipping Tool atau Win+Shift+S
- Screenshot terminal WSL yang menampilkan output program

### Format:
- Format: PNG
- Resolution: Minimal 1280x720
- Pastikan text jelas dan readable

---

**TODO**: @cathleenclau - Ambil screenshots setelah program berhasil di-compile dan dijalankan
