import socket

TCP_IP = '127.0.0.1'
TCP_PORT = 8080

def send_tcp_command(command):
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(3)  
            s.connect((TCP_IP, TCP_PORT))
            s.sendall(command.encode())
            response = s.recv(1024).decode()
            return response
    except Exception as e:
        return f"Erro: {e}"

def get_tcp_status():
    return send_tcp_command("status")
