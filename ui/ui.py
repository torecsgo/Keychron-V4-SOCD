import tkinter as tk
from tkinter import messagebox
import hid
import threading
import time

# Configuración del dispositivo HID
VENDOR_ID = 0x1234  # Cambia esto por el Vendor ID de tu teclado
PRODUCT_ID = 0x5678  # Cambia esto por el Product ID de tu teclado

# Variables globales para el estado del teclado
current_mode = None
snaptap_active = None

def send_command(command, value):
    try:
        with hid.Device(VENDOR_ID, PRODUCT_ID) as device:
            data = [command, value] + [0] * 62
            device.write(data)
    except Exception as e:
        messagebox.showerror("Error", f"No se pudo enviar el comando.\n{e}")

def query_state():
    global current_mode, snaptap_active
    try:
        with hid.Device(VENDOR_ID, PRODUCT_ID) as device:
            # Consultar iluminación
            device.write([0x03, 0x01] + [0] * 62)
            response = device.read(64, timeout_ms=500)
            if response and response[0] == 0x01:
                current_mode = response[1]

            # Consultar SnapTap
            device.write([0x03, 0x02] + [0] * 62)
            response = device.read(64, timeout_ms=500)
            if response and response[0] == 0x02:
                snaptap_active = bool(response[1])
    except Exception as e:
        print(f"Error al consultar estado: {e}")

def update_ui():
    # Actualizar botones LED
    if current_mode == 0:
        btn_apagar_leds.config(state="disabled")
        btn_bandera_espana.config(state="normal")
    elif current_mode == 1:
        btn_apagar_leds.config(state="normal")
        btn_bandera_espana.config(state="disabled")
    else:
        btn_apagar_leds.config(state="normal")
        btn_bandera_espana.config(state="normal")

    # Actualizar botones SnapTap
    if snaptap_active is None:
        btn_activar_snaptap.config(state="normal")
        btn_desactivar_snaptap.config(state="normal")
    elif snaptap_active:
        btn_activar_snaptap.config(state="disabled")
        btn_desactivar_snaptap.config(state="normal")
    else:
        btn_activar_snaptap.config(state="normal")
        btn_desactivar_snaptap.config(state="disabled")

def refresh_status():
    while True:
        query_state()
        update_ui()
        time.sleep(1)

# Crear la ventana principal
root = tk.Tk()
root.title("Control del Teclado")
root.geometry("300x250")
root.resizable(False, False)

# Etiqueta de título
title_label = tk.Label(root, text="Control del Teclado", font=("Helvetica", 16))
title_label.pack(pady=10)

# Botones para control de luces
led_frame = tk.Frame(root)
led_frame.pack(pady=5)

tk.Label(led_frame, text="Luces LED:").grid(row=0, column=0, pady=5)
btn_apagar_leds = tk.Button(led_frame, text="Apagar", command=lambda: send_command(0x01, 0))
btn_apagar_leds.grid(row=0, column=1, padx=5)

btn_bandera_espana = tk.Button(led_frame, text="Bandera de España", command=lambda: send_command(0x01, 1))
btn_bandera_espana.grid(row=0, column=2, padx=5)

# Botones para control de SnapTap
snaptap_frame = tk.Frame(root)
snaptap_frame.pack(pady=5)

tk.Label(snaptap_frame, text="SnapTap:").grid(row=0, column=0, pady=5)
btn_desactivar_snaptap = tk.Button(snaptap_frame, text="Desactivar", command=lambda: send_command(0x02, 0))
btn_desactivar_snaptap.grid(row=0, column=1, padx=5)

btn_activar_snaptap = tk.Button(snaptap_frame, text="Activar", command=lambda: send_command(0x02, 1))
btn_activar_snaptap.grid(row=0, column=2, padx=5)

# Botón de salir
exit_button = tk.Button(root, text="Salir", command=root.quit, bg="red", fg="white")
exit_button.pack(pady=10)

# Hilo para refrescar el estado
threading.Thread(target=refresh_status, daemon=True).start()

# Iniciar la interfaz gráfica
root.mainloop()