'''
senha: @BatatinhaFrita123
senha de aplicação: yoam nuaa qtow pqjt
Codigo para ler dados da serial COM3 a cada 2500 milissegundo e enviar para o email via SMTP.
e-mail: emailprojetoaoc@gmail.com
dados de entrada: temperatura, umidade, tensao, fumaca, porta
se no code vier 0, tudo ok
se no code vier 1, algum problema ocorreu
code: 10000 - tempteratura alta
code: 01000 - umidade alta ou baixa
code: 00100 - tensao fora do limite
code: 00010 - fumaca detectada
code: 00001 - porta aberta
Exemplo de dado recebido:
{'code': '00010', 'temperatura': 25.00, 'umidade': 50.00, 'tensao': 128.00, 'fumaca': 0, 'porta': 1}
enviar email se code != 00000
'''
try:
    import serial
except ImportError:
    print("Module 'serial' (pyserial) not found. Install with: pip install pyserial")
    serial = None

import time
import smtplib
from email.mime.text import MIMEText
from email.mime.multipart import MIMEMultipart
def enviar_email(mensagem):
    remetente = "emailprojetoaoc@gmail.com"
    destinatario = "emailprojetoaoc@gmail.com"
    senha = "yoam nuaa qtow pqjt"

    msg = MIMEMultipart()
    msg['From'] = remetente
    msg['To'] = destinatario
    msg['Subject'] = "Alerta de Monitoramento de Rack"

    msg.attach(MIMEText(mensagem, 'plain'))

    try:
        server = smtplib.SMTP('smtp.gmail.com', 587)
        server.starttls()
        server.login(remetente, senha)
        text = msg.as_string()
        server.sendmail(remetente, destinatario, text)
        server.quit()
        print("Email enviado com sucesso!")
    except Exception as e:
        print(f"Erro ao enviar email: {e}")