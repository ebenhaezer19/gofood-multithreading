# Quick Start Guide

## 🚀 Get Started in 5 Minutes

### Prerequisites
- WSL/Ubuntu or Linux system
- GCC compiler
- Make

### Step 1: Clone Repository
```bash
git clone https://github.com/[username]/gofood-multithreading.git
cd gofood-multithreading
```

### Step 2: Build
```bash
make
```

### Step 3: Run

#### See Race Condition (Data Inconsistency)
```bash
./gofood_system
```

#### See Fixed Version (Data Consistency)
```bash
./gofood_system --sync
```

### Step 4: Compare Results
```bash
make run-both
```

---

## 📊 What to Look For

### Race Condition Mode (Broken)
- ❌ Order count ≠ 30
- ❌ Revenue inconsistent
- ❌ Data corruption

### Synchronized Mode (Fixed)
- ✅ Order count = 30
- ✅ Revenue accurate
- ✅ Data consistent

---

## 🔧 Common Commands

```bash
# Build
make

# Run race condition demo
make run-race

# Run synchronized version
make run-sync

# Run both for comparison
make run-both

# Clean build
make clean

# Run tests
chmod +x test.sh
./test.sh
```

---

## 📚 Learn More

- Full documentation: [README.md](README.md)
- Setup guide: [docs/SETUP_GUIDE.md](docs/SETUP_GUIDE.md)
- Full report: [docs/LAPORAN.md](docs/LAPORAN.md)

---

## 🆘 Need Help?

Check [docs/SETUP_GUIDE.md](docs/SETUP_GUIDE.md) for troubleshooting.

---

**Happy coding! 💻**
