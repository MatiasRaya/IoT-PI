# Proyecto con LilyGO T-SIM7670G S3

Este proyecto utiliza la placa **LilyGO T-SIM7670G S3** para desarrollar una solución IoT basada en comunicación GSM, sensores, módulos de almacenamiento y sincronización de tiempo. Se incluye integración con **ThingsBoard** para monitoreo y gestión remota.

## 🔧 Hardware Utilizado

- **[LilyGO T-SIM7670G S3](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX)**  
  Repositorio oficial con ejemplos y documentación.

- **[Ficha del producto en tienda oficial LilyGO](https://lilygo.cc/products/t-sim-7670g-s3?srsltid=AfmBOoowFPeOL0-oDb2Hlp35UwYXt_EmSn-TTLXlu_u8921EcUAY8BVK)**

- **[RTC DS3231 con EEPROM AT24C32](https://tienda.ityt.com.ar/interfaz-memoria-rtc/3105-ds3231-at24c32-iic-precision-rtc-s-bateria-itytarg.html?search_query=ds3231)**  
  Módulo de reloj en tiempo real para sincronización precisa.

- **[Módulo LED RGB 10mm 4 pines](https://tienda.ityt.com.ar/modulo-leds/7391-modulo-140c05-led-rgb-10mm-4-pines-5v-itytarg.html)**  
  Para señales visuales de estado.

- **[Sensor de caudal YF-S201](https://tienda.ityt.com.ar/sensor-flujo-liquido/237-sensor-flujo-agua-caudalimetro-yf-s201-1-2-itytarg.html?search_query=caudalimetro)**  
  Sensor de flujo de agua con salida digital.

## 🧰 Librerías Utilizadas

- **[TinyGSM](https://github.com/vshymanskyy/TinyGSM)**  
  Comunicación con módems GSM/GPRS.

- **[TinyGPS](https://github.com/mikalhart/TinyGPS)**  
  Decodificación de datos GPS NMEA (opcional si se usa GPS).

- **[ArduinoHttpClient](https://github.com/arduino-libraries/ArduinoHttpClient)**  
  Cliente HTTP para realizar solicitudes REST.

## 🌐 Plataforma IoT

- **[ThingsBoard - Documentación de API HTTP](https://thingsboard.io/docs/reference/http-api/)**  
  Guía para enviar datos, atributos y telemetría.

- **[ThingsBoard - Atributos del dispositivo](https://thingsboard.io/docs/user-guide/attributes/)**  
  Uso de atributos para comunicación bidireccional.

## 🧪 Entorno de Desarrollo

- **[PlatformIO](https://docs.platformio.org/en/latest/)**  
  Framework y entorno para desarrollo embebido con ESP32, integración con VS Code.

## ⚙️ Cómo Compilar

1. **Instalar PlatformIO**  
   Descargar e instalar [VS Code](https://code.visualstudio.com/) y luego agregar la extensión **PlatformIO IDE** desde el marketplace.

2. **Clonar este repositorio**

   ```bash
   git clone https://github.com/MatiasRaya/IoT-PI.git
   cd IoT-PI
   ```

3. Y abrir el proyecto en VS Code

4. Verificar la configuración del archivo **platformio.ini**

5. **Instalar dependencias**  
   PlatformIO instalará automáticamente las librerías especificadas en platformio.ini

6. **Compilar el proyecto**  
   Desde VS Code:  
   **PlatformIO > Build**  
   o por terminal:

   ```bash
   pio run
   ```

7. **Subir el firmware a la placa**  
   Conectá la placa por USB y desde VS Code:  
   **PlatformIO > Upload**  
   o por terminal:

   ```bash
   pio run --target upload
   ```

8. **Abrir el monitor serial**  
   Desde VS Code:  
   **Platomio > Monitor**
   o por terminal:

   ```bash
   pio device monitor
   ```
