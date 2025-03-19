#include "gsm.h"

TinyGsm modem(SerialAT);
TinyGsmClient client(modem);
HttpClient http(client, "worldclockapi.com", 80);

bool checkSimCard()
{
    Serial.println("Verificando tarjeta SIM");

    SimStatus sim = SIM_ERROR;
    while (sim != SIM_READY) {
        sim = modem.getSimStatus();

        Serial.println("Estado de la tarjeta SIM: " + String(sim));

        switch (sim)
        {
            case SIM_READY:
                Serial.println("SIM card online");
                break;
            case SIM_LOCKED:
                Serial.println("The SIM card is locked. Please unlock the SIM card first.");
                break;
            default:
                break;
        }

        delay(1000);
    }

    if (modem.getSimStatus() == SIM_READY)
    {
        Serial.println("Tarjeta SIM detectada");
        return true;
    }
    else
    {
        Serial.println("Tarjeta SIM no detectada");
        return false;
    }
}

bool connetToNetwork(const char* apn)
{
    Serial.println("Conectando a la red");

    modem.gprsConnect(apn, "", "");

    bool connected = false;

    if (modem.waitForNetwork(60000))
    {
        Serial.println("Conectado a la red");

        String command = String("+CGDCONT=1,\"IP\",\"") + apn + String("\"");

        String response = sendATCommand(command.c_str(), 5000);

        Serial.println("Respuesta recibida: " + response);

        response = sendATCommand("+CGACT=1", 5000);

        Serial.println("Respuesta recibida: " + response);

        response = sendATCommand("+CEREG?", 5000);

        Serial.println("Respuesta recibida: " + response);

        response = sendATCommand("+CGATT?", 5000);

        Serial.println("Respuesta recibida: " + response);

        connected = true;
    }
    else
    {
        Serial.println("Error conectando a la red");
    }

    return connected;
}

String sendATCommand(const char* command, uint32_t timeout)
{
    modem.testAT(timeout);

    Serial.println("Comando AT a enviar: AT" + String(command));

    modem.sendAT(GF(String(command)));

    String responseData = "";
    
    modem.waitResponse(timeout, responseData);

    return responseData;
}

int getSignalQuality()
{
    return modem.getSignalQuality();
}

String getLocalIP()
{
    String localIP = modem.getLocalIP();

    int start = localIP.indexOf(":") + 2;
    int end = localIP.indexOf("\r");

    return localIP.substring(start, end);
}

bool sendHttpGetRequest()
{
    String response = sendATCommand("+CEREG?", 5000);

    Serial.println("Respuesta recibida: " + response);

    Serial.println("Realizando solicitud GET a worldclockapi.com...");

    http.get("/api/json/utc/now");
    int statusCode = http.responseStatusCode();
    response = http.responseBody();

    Serial.print("Código de respuesta: ");
    Serial.println(statusCode);
    Serial.println("Respuesta:");
    Serial.println(response);

    return statusCode == 200;
}

bool sendHttpPostRequest()
{
    String response = sendATCommand("+CREG?", 5000);

    Serial.println("Respuesta recibida: " + response);

    Serial.println("Realizando solicitud POST a httpbin.org...");

    const char* server = "httpbin.org";  // Servidor de httpbin
    const int port = 80;  // Puerto para HTTP (cambiar a 443 para HTTPS si es compatible)

    if (client.connect(server, port)) {
        Serial.println("Conectado al servidor");

        // Cabecera de la solicitud POST
        String postData = "name=JohnDoe&age=30";  // Datos a enviar en el cuerpo de la solicitud

        // Enviar solicitud POST
        client.print(String("POST ") + "/post" + " HTTP/1.1\r\n");
        client.print("Host: httpbin.org\r\n");
        client.print("Connection: close\r\n");
        client.print("Content-Type: application/x-www-form-urlencoded\r\n");
        client.print("Content-Length: " + String(postData.length()) + "\r\n");
        client.print("\r\n");  // Separador entre cabecera y cuerpo
        client.print(postData);  // Cuerpo de la solicitud (datos que quieres enviar)

        delay(5000);  // Esperar a que se envíe la solicitud

        // Leer la respuesta byte por byte
        String response = "";
        while (client.available()) {
            char c = client.read();  // Leer un byte
            response += c;  // Agregarlo a la respuesta
            // Serial.print(c);  // Imprimir cada byte inmediatamente
        }

        // Mostrar respuesta completa (si no se imprime bien con el loop anterior)
        // Serial.println("\nRespuesta completa:");
        Serial.println(response);
        // Serial.println("Fin de la respuesta");
        // Serial.println("Longitud de la respuesta: " + String(response.length()));
        
        client.stop();

        if (response.indexOf("200 OK") != -1) {
            Serial.println("Solicitud POST exitosa");

            return true;
        } else {
            Serial.println("Error en la solicitud POST");

            return false;
        }
    } else {
        Serial.println("Error al conectar con el servidor");
        client.stop();
        return false;
    }
}
