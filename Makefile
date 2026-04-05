# NOTA: se activa en la carpeta actual del archivo Makefile utilizando el comando 'make'
all: venv compile run

venv:
	@echo "Creando entorno virtual..."
	python3 -m venv venv
	@./venv/bin/pip install -r requirements.txt

# Compila codigo c
compile:
	@echo "Compilando programa en C..."
	gcc -Wall ppal_recta_args.c pfrep.c -o pf_bin

# Ejecuta la aplicación usando el Py del env
run:
	@echo "Lanzando aplicación..."
	@./venv/bin/python3 front_gui.py

# Limpia los archivos generados
clean:
	rm -rf venv pf_bin __pycache__
