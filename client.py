import socket

def send_command(ip, port, command):
    try:
        with socket.create_connection((ip, port), timeout=5) as sock:
            sock.sendall(command.encode('utf-8'))
            
            # Recebe a resposta (até 4096 bytes, pode ajustar o tamanho)
            response = sock.recv(4096)
            if response:
                print(f"Resposta do servidor: {response.decode('utf-8')}")
            else:
                print("Nenhuma resposta recebida do servidor.")
            
            print(f"Comando '{command}' enviado com sucesso para {ip}:{port}")
    except Exception as e:
        print(f"Erro ao enviar comando: {e}")

if __name__ == "__main__":
    import sys

    if len(sys.argv) != 4:
        print("Uso: python client.py <IP> <PORTA> <COMANDO>")
        print("Exemplo: python client.py 192.168.0.10 8080 activate")
        sys.exit(1)

    ip = sys.argv[1]
    port = int(sys.argv[2])
    command = sys.argv[3]

    send_command(ip, port, command)
