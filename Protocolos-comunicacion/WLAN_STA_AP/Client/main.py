from connection import WiFi
import usocket

# Se crea un objeto de la clase WiFi
wifi = WiFi()

# Se configura el dispositivo como estación
wifi.config_STA("Prueba", "PruebadeAP", "ext")

# Se imprime la direccion ip
print(wifi.wlan.ifconfig())

# Se crea un socket
s = usocket.socket(usocket.AF_INET, usocket.SOCK_STREAM)

# Se conecta el socket al servidor
s.connect(("192.168.4.1", 8080))

# Se intrsa un mensaje por teclado
msg = input("Mensaje: ")
# Se envia el mensaje al servidor
s.send(msg)

# Se recibe la respuesta del servidor
print("Response", s.recv(1024))

# Se cierra el socket
s.close()