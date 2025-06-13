import socket
import json

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
        return (f"Erro: {e}")

def get_tcp_status():
    try:
        with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
            s.settimeout(3)  
            s.connect((TCP_IP, TCP_PORT))
            s.sendall(b'status')

            # Recebe o JSON do servidor 
            data = s.recv(4096)
            s.close()

            json_data = json.loads(data.decode())

            json_data = processar_dados_sensor(json_data)

            print(json_data)
            return json_data
    except Exception as e:
        print(f"Erro: {e}")
        return (f"Erro: {e}")

def processar_dados_sensor(json_data):
    """
    Recebe o JSON com dados crus e retorna o mesmo JSON com:
    - umidade em porcentagem
    - tensão lida do sensor
    """
    try:
        leitura_umidade = json_data['systemStatus']['moisture']
        leitura_nivel = json_data['systemStatus']['waterLevel']

        # Parâmetros calibrados por Arthur
        leitura_seco = 20500
        leitura_molhado = 10500
        leitura_vazio = 17550
        leitura_cheio = 0

        # Conversões
        tensao_umidade = leitura_umidade * 2.048 / 32767.0
        umidade_percentual = (leitura_seco - leitura_umidade) * 100.0 / (leitura_seco - leitura_molhado)
        umidade_percentual = max(0.0, min(umidade_percentual, 100.0))

        tensao_nivel = leitura_nivel * 2.048 / 32767.0
        nivel_percentual = (leitura_nivel - leitura_vazio) * 100.0 / (leitura_cheio - leitura_vazio)
        nivel_percentual = max(0.0, min(nivel_percentual, 100.0))

        # Atualiza JSON com dados convertidos
        json_data['systemStatus'].update({
            'moisture_percent': round(umidade_percentual, 1),
            'moisture_voltage': round(tensao_umidade, 3),
            'waterLevel_percent': round(nivel_percentual, 1),
            'waterLevel_voltage': round(tensao_nivel, 3),
            'status_readable': traduzir_status(json_data['systemStatus'].get('status', ''))
        })

        return json_data

    except KeyError as e:
        print(f"Erro: chave ausente no JSON: {e}")
        return json_data
    except Exception as e:
        print(f"Erro ao processar dados do sensor: {e}")
        return json_data
