import tkinter as tk
from tkinter import messagebox, scrolledtext
import socket
import os

TCP_IP = '192.168.163.40'
TCP_PORT = 8080
LOG_FILE = 'automonitor.log'

# Comunicação TCP
def send_tcp_command(command):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.connect((TCP_IP, TCP_PORT))
            s.sendall(command.encode())
            return f"Comando '{command}' enviado com sucesso."
    except Exception as e:
        return f"Erro: {e}"

def ativar_rega():
    resposta = send_tcp_command("activate")
    atualizar_resposta(resposta)

def parar_bomba():
    resposta = send_tcp_command("stop")
    atualizar_resposta(resposta)

def verificar_status():
    resposta = send_tcp_command("status")
    atualizar_resposta(resposta)

def atualizar_resposta(msg):
    resposta_text.config(state='normal')
    resposta_text.delete(1.0, tk.END)
    resposta_text.insert(tk.END, msg)
    resposta_text.config(state='disabled')

# Interface
def criar_interface():
    root = tk.Tk()
    root.title("🌿 Sistema de Irrigação TCP")
    root.geometry("500x600")
    root.configure(bg="#d5f1dd")

    # Cabeçalho
    header = tk.Label(root, text="🌱 Sistema de Irrigação Inteligente", font=("Segoe UI", 18, "bold"), bg="#d5f1dd", fg="#2e7d32")
    header.pack(pady=10)

    # Info frame
    info_frame = tk.Frame(root, bg="#e3f7e8", bd=2, relief="groove")
    info_frame.pack(padx=20, pady=10, fill="x")

    def add_info(label, value):
        row = tk.Frame(info_frame, bg="#e3f7e8")
        row.pack(anchor="w", pady=4, padx=10)
        tk.Label(row, text=label, font=("Segoe UI", 12, "bold"), bg="#e3f7e8", fg="#333").pack(side="left")
        tk.Label(row, text=value, font=("Segoe UI", 12), bg="#e3f7e8", fg="#555").pack(side="left")

    add_info("💧 Umidade do Solo: ", "43%")
    add_info("🚰 Nível do Reservatório: ", "Cheio")
    add_info("⚙️ Estado da Bomba: ", "Desligada")
    add_info("🌐 Estado do Sistema: ", "Online")
    add_info("📊 Ativações Hoje: ", "3")

    # Botões de comando
    cmd_frame = tk.LabelFrame(root, text="🧭 Comandos", font=("Segoe UI", 12, "bold"), bg="#e3f7e8", fg="#2e7d32")
    cmd_frame.pack(padx=20, pady=10, fill="x")

    tk.Button(cmd_frame, text="✅ STATUS", command=verificar_status, bg="#a5d6a7", font=("Segoe UI", 12, "bold")).pack(side="left", expand=True, fill="x", padx=5, pady=10)
    tk.Button(cmd_frame, text="💦 REGAR", command=ativar_rega, bg="#81c784", font=("Segoe UI", 12, "bold")).pack(side="left", expand=True, fill="x", padx=5, pady=10)
    tk.Button(cmd_frame, text="🛑 PARAR", command=parar_bomba, bg="#e57373", font=("Segoe UI", 12, "bold")).pack(side="left", expand=True, fill="x", padx=5, pady=10)

    # Resposta
    resposta_label = tk.Label(root, text="📥 Última Resposta", font=("Segoe UI", 12, "bold"), bg="#d5f1dd", fg="#2e7d32")
    resposta_label.pack(pady=(20, 5))

    global resposta_text
    resposta_text = scrolledtext.ScrolledText(root, height=5, font=("Consolas", 11), state="disabled", wrap=tk.WORD)
    resposta_text.pack(padx=20, fill="x")

    # Rodapé
    tk.Label(root, text="🌿 Projeto Engenharia - v1.0", font=("Segoe UI", 10), bg="#d5f1dd", fg="gray").pack(side="bottom", pady=10)

    root.mainloop()

if __name__ == "__main__":
    criar_interface()
