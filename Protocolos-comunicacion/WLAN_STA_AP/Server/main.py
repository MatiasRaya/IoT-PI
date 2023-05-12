from connection import WiFi
import usocket

# Se crea un objeto de la clase WiFi
wifi = WiFi()

# Se configura el dispositivo como estación y access point
wifi.config_STA_AP("RAYADOS 2.4", "Rayaplasencia1996", "Prueba", "PruebadeAP", "ext")

# Se imprime la dirección IP de la estación
print(wifi.wlan.ifconfig())

# Se imprime la dirección IP del access point
print(wifi.wlan.ifconfig(id=1))

# Se crea un socket
s = usocket.socket(usocket.AF_INET, usocket.SOCK_STREAM)

# Se enlaza el socket a la dirección IP del access point y al puerto 80
s.bind(('0.0.0.0', 8080))

# Se pone el socket en modo escucha
s.listen(5)

# Se imprime un mensaje de que se está en modo escucha
print("Listening...")

# Se recibe la información del cliente
while True:
    # Se configura el cliente y la dirección IP del cliente
    client, addr = s.accept()

    # Se imprime un mensaje de que se conectó un cliente y su addr
    print("Client connect of", addr)

    # Se recibe la información del cliente
    data = client.recv(1024)

    # Se verifica que la información no esté vacía
    if not data:
        break
    
    # Se imprime la información recibida
    print("Message: ", data.decode("utf-8"))

    # Se ingresa una respuesta por teclado
    response = input("Response: ")
    # Se envía la respuesta al cliente
    client.send(response.encode())

# Se cierra el socket
client.close()
# s.close()