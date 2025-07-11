
# Project with LilyGO T-SIM7670G S3

This project uses the **LilyGO T-SIM7670G S3** board to develop an IoT solution based on GSM communication, sensors, storage modules, and time synchronization. It includes integration with **ThingsBoard** for remote monitoring and management.

## 🔧 Hardware Used

- **[LilyGO T-SIM7670G S3](https://github.com/Xinyuan-LilyGO/LilyGO-T-A76XX)**  
  Official repository with examples and documentation.

- **[Product page on official LilyGO store](https://lilygo.cc/products/t-sim-7670g-s3?srsltid=AfmBOoowFPeOL0-oDb2Hlp35UwYXt_EmSn-TTLXlu_u8921EcUAY8BVK)**

- **[RTC DS3231 with EEPROM AT24C32](https://tienda.ityt.com.ar/interfaz-memoria-rtc/3105-ds3231-at24c32-iic-precision-rtc-s-bateria-itytarg.html?search_query=ds3231)**  
  Real-time clock module for accurate time synchronization.

- **[10mm RGB LED Module, 4 pins](https://tienda.ityt.com.ar/modulo-leds/7391-modulo-140c05-led-rgb-10mm-4-pines-5v-itytarg.html)**  
  For visual status indicators.

- **[YF-S201 Flow Sensor](https://tienda.ityt.com.ar/sensor-flujo-liquido/237-sensor-flujo-agua-caudalimetro-yf-s201-1-2-itytarg.html?search_query=caudalimetro)**  
  Water flow sensor with digital output.

## 🧰 Libraries Used

- **[TinyGSM](https://github.com/vshymanskyy/TinyGSM)**  
  Communication with GSM/GPRS modems.

- **[TinyGPS](https://github.com/mikalhart/TinyGPS)**  
  Decoding GPS NMEA data (optional if GPS is used).

- **[ArduinoHttpClient](https://github.com/arduino-libraries/ArduinoHttpClient)**  
  HTTP client for making REST requests.

- **[ArduinoJson](https://github.com/bblanchon/ArduinoJson)**  
  Efficient JSON parsing and serialization library for embedded systems.

- **[StreamDebugger](https://github.com/vshymanskyy/StreamDebugger)**  
  Debugging utility to monitor and log AT commands sent to GSM modules.


## 🌐 IoT Platform

- **[ThingsBoard - HTTP API Documentation](https://thingsboard.io/docs/reference/http-api/)**  
  Guide to send data, attributes, and telemetry.

- **[ThingsBoard - Device Attributes](https://thingsboard.io/docs/user-guide/attributes/)**  
  Using attributes for bidirectional communication.

## ⚙️ RTOS Support

This project is designed to support multitasking using **FreeRTOS**, which is built into the ESP32 core.

- **[FreeRTOS - Tasks and Co-routines Documentation](https://www.freertos.org/Documentation/02-Kernel/02-Kernel-features/01-Tasks-and-co-routines/00-Tasks-and-co-routines)**  
  Overview of task management and co-routines in FreeRTOS.

## 🧪 Development Environment

- **[PlatformIO](https://docs.platformio.org/en/latest/)**  
  Framework and environment for embedded development with ESP32, integrated with VS Code.

## ⚙️ How to Build

1. **Install PlatformIO**  
   Download and install [VS Code](https://code.visualstudio.com/), then add the **PlatformIO IDE** extension from the marketplace.

2. **Clone this repository**

   ```bash
   git clone https://github.com/MatiasRaya/IoT-PI.git
   cd IoT-PI
   ```

3. Open the project in VS Code

4. Check the **platformio.ini** configuration file

5. **Install dependencies**  
   PlatformIO will automatically install the libraries specified in platformio.ini

6. **Build the project**  
   In VS Code:  
   **PlatformIO > Build**  
   Or via terminal:

   ```bash
   pio run
   ```

7. **Upload the firmware to the board**  
   Connect the board via USB and in VS Code:  
   **PlatformIO > Upload**  
   Or via terminal:

   ```bash
   pio run --target upload
   ```

8. **Open the serial monitor**  
   In VS Code:  
   **PlatformIO > Monitor**  
   Or via terminal:

   ```bash
   pio device monitor
   ```
