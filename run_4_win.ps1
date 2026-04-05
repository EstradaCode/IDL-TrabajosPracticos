# Script de ejecución para Windows (PowerShell)

Write-Host "==> Verificando Entorno Virtual..." -ForegroundColor Cyan
if (!(Test-Path "venv")) {
    python -m venv venv
}

Write-Host "==> Instalando dependencias..." -ForegroundColor Cyan
.\venv\Scripts\pip install customtkinter

Write-Host "==> Compilando motor en C..." -ForegroundColor Cyan
# En Windows, GCC suele generar un .exe
gcc -Wall motor.c pfrep.c -o pf_bin.exe

Write-Host "==> Lanzando Interfaz Gráfica..." -ForegroundColor Green
.\venv\Scripts\python front_gui_pro.py
