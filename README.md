# 📈 Graficadora de Recta y=mx +b - Grupo 07 - INTRODUCCION AL DISEÑO LOGICO

Software de visualización de funciones lineales con cálculo de punto fijo desarrollado en **C** sin LIBRERIAS de flotantes, y con interfaz gráfica en **Python**.

---

## 🚀 Instalación y Uso Rápido (Windows/Linux)

No es necesario instalar Python ni compiladores adicionales. Los ejecutables autocontenidos están listos para usarse en la carpeta `/bin`.

### Ejecución:
1. Ir a la carpeta `bin/`.
2. Ejecuta `Graficadora_Recta_G07.exe` (Windows) o `./Graficadora_Recta_G07_Linux` (Linux).

> **⚠️ Nota en Windows:** Es posible que Microsoft Defender muestre una advertencia ("Windows protegió su PC"). Para iniciar, haz clic en **"Más información"** y luego en **"Ejecutar de todas formas"**.
SE DEBE **EJECUTAR COMO ADMINISTRADOR** para que el binario .exe funcione apropiadamente sin latencia.
---

## 🛠️ Arquitectura del Sistema

* **Frontend (Python 3.14):** Desarrollado con la librería `customtkinter` para una interfaz moderna. Gestiona la lógica de la GUI y el renderizado de gráficos.
* **Logica Binaria (C):** Un binario nativo en lenguaje C (`pf_bin`) que realiza los cálculos matemáticos utilizando aritmética de punto fijo, garantizando precisión sin depender de librerias de punto flotante, ya que realizamos la logica binaria bit a bit en nuestra propia implementacion.



---

## ⚙️ Compilación y Desarrollo

En caso de querer compilarlo y correrlo:

### Requisitos
* **Python 3.11+** (con entorno virtual recomendado).
* **GCC** (MinGW-w64 para Windows o GCC nativo en Linux).

### Compilación del Motor de C
Para generar el motor de cálculo:
```bash
gcc -O3 -static pfrep.c ppal_recta_args.c -o pf_bin
