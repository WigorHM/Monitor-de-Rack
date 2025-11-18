import serial
import requests
import time

# --- Configurações ---
SERIAL_PORTA = 'COM3'  # Altere para a porta serial correta, se necessário
SERIAL_BAUDRATE = 9600  # Certifique-se de que a taxa de transmissão corresponda ao seu dispositivo
API_URL = 'http://sua-api.com/endpoint'  # **Substitua pela URL real da sua API**
TIMEOUT_SERIAL = 1     # Timeout para leitura serial (segundos)

def ler_e_enviar():
    """
    Configura a porta serial, lê dados e os envia para a API.
    """
    try:
        # 1. Configurar a porta serial
        ser = serial.Serial(
            port=SERIAL_PORTA,
            baudrate=SERIAL_BAUDRATE,
            timeout=TIMEOUT_SERIAL
        )
        print(f"✅ Porta serial {SERIAL_PORTA} aberta com sucesso.")

        while True:
            # 2. Ler dados da porta serial
            # A função readline() espera até receber um caractere de nova linha (\n)
            linha_bytes = ser.readline()
            
            # Decodificar os bytes para string (removendo espaços em branco/nova linha)
            if linha_bytes:
                # Usa .decode() para converter bytes para string, e .strip() para remover \r\n, etc.
                dados_recebidos = linha_bytes.decode('utf-8').strip()
                
                if dados_recebidos:
                    print(f"\n📨 Dados recebidos: '{dados_recebidos}'")

                    # 3. Preparar os dados para a API
                    # Criamos um dicionário (JSON) com a string recebida
                    payload = {
                        'valor_serial': dados_recebidos,
                        'timestamp': time.time()
                    }

                    try:
                        # 4. Enviar os dados para a API
                        print(f"🌐 Enviando para API em {API_URL}...")
                        response = requests.post(API_URL, json=payload)

                        # 5. Verificar a resposta da API
                        if response.status_code == 200 or response.status_code == 201:
                            print(f"👍 Sucesso! Status: {response.status_code}")
                            print(f"   Resposta da API: {response.text[:50]}...") # Exibe parte da resposta
                        else:
                            print(f"⚠️ Erro ao enviar. Status: {response.status_code}")
                            print(f"   Mensagem de erro: {response.text}")

                    except requests.exceptions.RequestException as e:
                        print(f"❌ Erro de conexão com a API: {e}")
                        
            # Pausa curta para evitar consumo excessivo de CPU
            time.sleep(0.1)

    except serial.SerialException as e:
        print(f"❌ Erro na porta serial: {e}")
        print("Certifique-se de que a porta COM3 não está sendo usada por outro programa e que o dispositivo está conectado.")
    except KeyboardInterrupt:
        print("\nPrograma encerrado pelo usuário (Ctrl+C).")
    finally:
        # Garante que a porta serial seja fechada
        if 'ser' in locals() and ser.is_open:
            ser.close()
            print("Porta serial fechada.")

if __name__ == "__main__":
    ler_e_enviar()