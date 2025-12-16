@echo off
echo ========================================
echo Testing main_scheduler.c
echo ========================================
echo.

REM Cek apakah gcc tersedia
where gcc >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: gcc tidak ditemukan!
    echo.
    echo Silakan install MinGW atau gunakan WSL/Linux
    echo Atau compile manual dengan compiler yang tersedia
    pause
    exit /b 1
)

echo [1/3] Compiling main_scheduler.c...
gcc -o scheduler.exe main_scheduler.c -Wall
if %ERRORLEVEL% NEQ 0 (
    echo ERROR: Compilation failed!
    pause
    exit /b 1
)
echo Compilation successful!
echo.

echo [2/3] Running scheduler...
echo.
scheduler.exe
echo.

echo [3/3] Testing complete!
echo.
echo Output telah ditampilkan di atas.
echo Pastikan:
echo   - Semua 30 task selesai
echo   - Rata-rata waiting time multi-worker < single worker
echo   - Rata-rata turnaround time multi-worker < single worker
echo.

pause

