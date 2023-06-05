# Server.py by Dinlon5566
import socket
import threading
import select


class CommandServer:
    def __init__(self, host='0.0.0.0', port=8000):
        self.host = host
        self.port = port

    def start(self):
        print(f"[*] Starting server...")
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        server.bind((self.host, self.port))
        server.listen(5)
        print(f'[*] Listening on {self.host}:{self.port}')

        while True:
            client, address = server.accept()
            print(f'[*] Accepted connection from {address[0]}:{address[1]}')
            print(f"[*] press ^!stop to stop the server")
            client_handler = threading.Thread(
                target=self.handle_client, args=(client,))
            client_handler.start()

    def handle_client(self, client_socket):
        while True:
            try:
                # input command
                command = input("Enter the command to execute: ")
                if command == "^!stop":
                    print("[*] Stopping server...")
                    client_socket.close()
                    break
                client_socket.send(command.encode())

                # set timeout
                ready = select.select([client_socket], [], [], 5)
                if ready[0]:
                    output = client_socket.recv(1024)
                    # here can't work :'( 
                    if len(output) == 0:
                        print("Connection closed by remote host.")
                        break
                    try:
                        # utf-8 decode
                        print(f'Output: {output.decode("utf-8")}')
                    except UnicodeDecodeError:
                        # if utf-8 decode failed, try cp950 
                        print(f'Output: {output.decode("cp950")}')
                else:
                    print("Command timeout.")
            except Exception as e:
                print(f"An error occurred: {e}")


if __name__ == '__main__':
    # Initialize and start our server
    server = CommandServer()
    server.start()
