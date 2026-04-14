import customtkinter as ctk
import subprocess
import os      
import sys     
import platform
from tkinter import messagebox
def get_resource_path(relative_path):
    """ Obtiene la ruta del binario, compatible con desarrollo y PyInstaller """
    if getattr(sys, 'frozen', False):
        # Si corre .exe, la carpeta temporal es sys._MEIPASS
        base_path = sys._MEIPASS
    else:
        # Si corre en modo normal (python front_gui.py)
        base_path = os.path.dirname(os.path.abspath(__file__))
    return os.path.join(base_path, relative_path)
ctk.set_appearance_mode("dark")
ctk.set_default_color_theme("blue")

class App(ctk.CTk):
    def __init__(self):
        super().__init__()
        self.title("UNLP - GRUPO07 Representación punto fijo Q(16,15)")
        self.geometry("1000x600")

        # Variables de estado para el gráfico
        self.zoom = 5.0
        self.pan_x = 300  # Empezamos centrados en el canvas de 600
        self.pan_y = 150  # Empezamos centrados en el canvas de 300
        self.last_m = 0.0
        self.last_b = 0.0
        self.last_x_in = 0.0
        self.last_y_out = 0.0

        # Layout
        self.grid_columnconfigure(1, weight=1)
        self.grid_rowconfigure(0, weight=1)

        # --- SIDEBAR ---
        self.sidebar = ctk.CTkFrame(self, width=300)
        self.sidebar.grid(row=0, column=0, sticky="nsew", padx=20, pady=20)
        
        ctk.CTkLabel(self.sidebar, text="Variables", font=("Arial", 20, "bold")).pack(pady=20)

        self.entry_m = self.create_input("Pendiente m [-1, 1)", "0.5")
        self.entry_b = self.create_input("Ordenada b [-128, 128)", "1.25")
        self.entry_x = self.create_input("Punto x [-65407, 65407]", "10")

        self.btn_calc = ctk.CTkButton(self.sidebar, text="CALCULAR", command=self.calcular, 
                                      fg_color="#2ecc71", hover_color="#27ae60")
        self.btn_calc.pack(pady=20, padx=20, fill="x")

        # --- MAIN ---
        self.main_frame = ctk.CTkFrame(self, fg_color="transparent")
        self.main_frame.grid(row=0, column=1, sticky="nsew", padx=20, pady=20)

        self.canvas = ctk.CTkCanvas(self.main_frame, height=350, bg="#1a1a1a", highlightthickness=0)
        self.canvas.pack(fill="x", pady=(0, 20))

        # Controles de Zoom Visual
        self.zoom_slider = ctk.CTkSlider(self.main_frame, from_=0.1, to=100, command=self.cambio_zoom_slider)
        self.zoom_slider.set(self.zoom)
        self.zoom_slider.pack(fill="x", padx=10)
        ctk.CTkLabel(self.main_frame, text="Zoom Visual (Rueda del mouse o Slider)").pack()

        self.res_frame = ctk.CTkFrame(self.main_frame)
        self.res_frame.pack(fill="both", expand=True, pady=10)

        self.lbl_hex = ctk.CTkLabel(self.res_frame, text="Hexadecimal: -", font=("Courier", 16))
        self.lbl_hex.pack(pady=5)
        self.lbl_dec = ctk.CTkLabel(self.res_frame, text="Esperando entrada...", font=("Arial", 22, "bold"), text_color="#3498db")
        self.lbl_dec.pack(pady=5)

        # Eventos de Mouse
        self.canvas.bind("<B1-Motion>", self.arrastrar_mapa)
        self.canvas.bind("<Button-1>", self.guardar_pos_mouse)
        self.canvas.bind("<MouseWheel>", self.aplicar_zoom_mouse) # Windows/macOS
        self.canvas.bind("<Button-4>", self.aplicar_zoom_mouse)   # Linux scroll up
        self.canvas.bind("<Button-5>", self.aplicar_zoom_mouse)   # Linux scroll down

    def create_input(self, label_text, default):
        lbl = ctk.CTkLabel(self.sidebar, text=label_text)
        lbl.pack(padx=20, anchor="w")
        entry = ctk.CTkEntry(self.sidebar)
        entry.insert(0, default)
        entry.pack(padx=20, pady=(0, 15), fill="x")
        return entry

    # --- LÓGICA DE CONTROL ---
    def guardar_pos_mouse(self, event):
        self.last_mouse_x = event.x
        self.last_mouse_y = event.y

    def arrastrar_mapa(self, event):
        self.pan_x += (event.x - self.last_mouse_x)
        self.pan_y += (event.y - self.last_mouse_y)
        self.last_mouse_x = event.x
        self.last_mouse_y = event.y
        self.actualizar_grafico(self.last_m, self.last_b, self.last_y_out, self.last_x_in)

    def aplicar_zoom_mouse(self, event):
        if event.num == 4 or event.delta > 0: self.zoom *= 1.1
        else: self.zoom /= 1.1
        self.zoom = max(0.001, min(self.zoom, 500)) # Limites de zoom
        self.zoom_slider.set(self.zoom)
        self.actualizar_grafico(self.last_m, self.last_b, self.last_y_out, self.last_x_in)

    def cambio_zoom_slider(self, value):
        self.zoom = float(value)
        self.actualizar_grafico(self.last_m, self.last_b, self.last_y_out, self.last_x_in)

    def validar_entradas(self):
        try:
            m = float(self.entry_m.get())
            b = float(self.entry_b.get())
            x = float(self.entry_x.get())
            if not (-1.0 <= m <= 0.99997): raise ValueError("m fuera de rango Q0.15")
            if not (-128.0 <= b <= 127.996): raise ValueError("b fuera de rango Q7.8")
            if not (-65407.0 <= x <= 65407.0): raise ValueError("x fuera de rango (Overflow)")
            return m, b, x
        except ValueError as e:
            messagebox.showwarning("Entrada Inválida", str(e))
            return None

    def calcular(self):
        vals = self.validar_entradas()
        if not vals: return
        
        m, b, x_in = vals
        try:
            # Llamada al binario 

            nombre_binario = "pf_bin.exe" if platform.system() == "Windows" else "pf_bin"
            ruta_binario = get_resource_path(nombre_binario)

            res = subprocess.check_output([ruta_binario, str(m), str(b), str(x_in)], text=True)
            y_hex, y_dec_str = res.strip().split(',')
            y_dec = float(y_dec_str)

            # Actualizar labels
            self.lbl_hex.configure(text=f"Hexadecimal: {y_hex}")
            self.lbl_dec.configure(text=f"Resultado Y: {y_dec:.5f}")

            # Guardar estado para redibujado
            self.last_m, self.last_b, self.last_x_in, self.last_y_out = m, b, x_in, y_dec

            # AUTO-CENTRADO: Centrar la cámara en el nuevo punto (x, y)
            canvas_w = 600 # Ancho aproximado del canvas
            canvas_h = 350 # Alto aproximado
            self.pan_x = (canvas_w / 2) - (x_in * self.zoom)
            self.pan_y = (canvas_h / 2) + (y_dec * self.zoom)

            self.actualizar_grafico(m, b, y_dec, x_in)
        except Exception as e:
            messagebox.showerror("Error", f"Fallo al ejecutar motor.c: {e}")

    def actualizar_grafico(self, m, b, y_act, x_act):
        self.canvas.delete("all")
        w = self.canvas.winfo_width()
        h = self.canvas.winfo_height()
        if w < 10: w, h = 600, 350 # Fallback inicial

        orig_x, orig_y = self.pan_x, self.pan_y

        # Grilla milimetrada dinámica
        grid_size = 50 * (self.zoom / 5.0)
        if grid_size > 5: # No dibujar si es demasiado densa
            for i in range(int(orig_x % grid_size), w, int(grid_size)):
                self.canvas.create_line(i, 0, i, h, fill="#222222")
            for i in range(int(orig_y % grid_size), h, int(grid_size)):
                self.canvas.create_line(0, i, w, i, fill="#222222")

        # Ejes
        self.canvas.create_line(0, orig_y, w, orig_y, fill="#555555", width=1) # Eje X
        self.canvas.create_line(orig_x, 0, orig_x, h, fill="#555555", width=1) # Eje Y

        # Recta infinita calculada
        try:
            x_left = (0 - orig_x) / self.zoom
            x_right = (w - orig_x) / self.zoom
            y_left = m * x_left + b
            y_right = m * x_right + b
            self.canvas.create_line(0, orig_y - y_left * self.zoom, w, orig_y - y_right * self.zoom, 
                                    fill="#3498db", width=3)
        except: pass

        # Punto Y calculado
        px = orig_x + (x_act * self.zoom)
        py = orig_y - (y_act * self.zoom)
        self.canvas.create_oval(px-6, py-6, px+6, py+6, fill="#e74c3c", outline="white", width=2)
        self.canvas.create_text(px+15, py-15, text=f"PUNTO Y\n({x_act}, {y_act:.2f})", 
                                fill="white", font=("Arial", 10, "bold"), anchor="w")

if __name__ == "__main__":
    app = App()
    app.mainloop()