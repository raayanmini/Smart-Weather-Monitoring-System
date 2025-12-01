# Smart Weather Monitor System (SWMS) – Monochrome LCD + LM35

This version of SWMS runs on **STM32F401RBT6 (RaayanMini)** with a **16x2 monochrome character LCD** and an **LM35 analog temperature sensor**.

It reads temperature from the **LM35** via ADC, shows **time, date and temperature** on a **16x2 HD44780-compatible LCD**, and periodically pushes the current temperature to a remote server using an **ESP8266** (AT commands over UART).  
An external **I²C RTC** keeps track of date/time, and there is basic **offline logging** to an I²C EEPROM/FRAM when WiFi is down. All the logic is split into **FreeRTOS tasks**, so different functionalities run in parallel instead of one big blocking loop.

This is a **prototyping firmware developed by Kernel Masters students** – the goal is to explore ADC, I²C, UART, RTOS basics and simple IoT-style communication on STM32F4, not to build a production-grade weather station.


## 🛠 Hardware & Tools

-   **MCU Board**: STM32F401RBT6 (**RaayanMini**, LQFP64)
    
-   **Temperature Sensor**: LM35 (analog, 10 mV/°C) → connected to ADC1
    
-   **Display**: 16x2 **monochrome character LCD** (HD44780-compatible) in 4-bit mode
    
-   **RTC**: DS1307/DS3231 module over **I²C1** (`0x68`)
    
-   **Non-Volatile Log (optional)**: I²C EEPROM (`0x50`) for basic offline logging
    
-   **WiFi Module**: ESP8266 in AT-command mode on UART
    
-   **Status LEDs**: RED/GREEN on GPIOs for WiFi status indication
    
-   **RTOS**: CMSIS-RTOS (FreeRTOS backend)
    
-   **IDE / Toolchain**: STM32CubeIDE v1.18.0
    
-   **Project config**: `.ioc` file (CubeMX configuration) included in the repo
    

Exact pin mapping is configured inside the **`.ioc`** file and generated code; that is the single source of truth for pins and peripherals.


## 🔧 What the Firmware Does (High Level)

1.  **Power-On & Init**
    
    -   Initializes HAL, clocks and peripherals from the CubeMX config.
        
    -   Brings up the **16x2 LCD**, shows a simple splash screen.
        
    -   Checks for **RTC presence** on I²C (`0x68`). If not found, it reports the error on LCD and stops.
        
    -   Initializes the **ESP8266** with a sequence of AT commands (echo off, station mode, join AP, etc.).
        
    -   If WiFi init fails (module not found / wrong SSID/password), it prints a message on LCD and waits.
        
2.  **Runtime Behaviour (After RTOS Starts)**
    
    -   **Task 1 – Display (`dispDT`)**  
        Reads time/date from RTC over I²C and updates the LCD with `HH:MM:SS` and `DD/MM/YY`.  
        When a fresh temperature value is available, it also updates the `T:xx°C` field on the LCD.
        
    -   **Task 2 – Temperature (`readTemp`)**  
        Samples the LM35 via ADC at a fixed interval, converts ADC counts to a temperature value, and signals other tasks using a binary semaphore.
        
    -   **Task 3 – WiFi Status & Logging (`checkIntStatus`)**  
        Periodically checks `AT+CIPSTATUS`.
        
        -   If WiFi is OK, it lets the upload task run.
            
        -   If WiFi is down, it logs a simple record (timestamp + temp) to an I²C EEPROM.
            
    -   **Task 4 – Upload (`upTemp`)**  
        When WiFi is OK and a new temperature is ready, it opens a TCP connection and sends a simple HTTP GET with the latest temperature using the ESP8266.
        
3.  **Status Indication**
    
    -   A pair of LEDs (RED / GREEN) show WiFi link status (disconnected / OK).
        
    -   The LCD always shows current **time, date, and temperature**, so the system is still useful even if WiFi is not working.
        


## 🧵 RTOS Task Layout

| Task Name | What It Does | Notes |
| --- | --- | --- |
| `dispDT` | Reads RTC, updates LCD (time/date/temp) | Uses I²C RTC + LCD driver |
| `readTemp` | Samples LM35 via ADC and computes °C | Runs periodically, sets “new temp” flag |
| `checkIntStatus` | Checks WiFi status + decides upload/log path | Uses `AT+CIPSTATUS`, toggles LEDs |
| `upTemp` | Sends HTTP GET with temperature over ESP8266 | Runs only when WiFi OK + new temp present |

Binary semaphores are used for synchronization between **sensor**, **status**, and **upload** tasks.

* * *

## 🚀 Getting Started

### 1\. Clone the Repository

`Download the repository as .zip`  
`Extract the .zip file`  


### 2\. Import into STM32CubeIDE

1.  Open **STM32CubeIDE v1.18.0**
    
2.  Go to **File → Open Projects from File System…**
    
3.  Point it to the project root and import.
    
4.  You should see the project with its **`.ioc`** file.


### 2\.5\. Regenerate HAL Drivers / Middleware (Important)

The repo’s `.gitignore` usually **excludes auto-generated** HAL, CMSIS and Middleware sources.  
If `Drivers/` and `Middlewares/` are missing or incomplete:

1.  Open the `.ioc` file in STM32CubeIDE.
    
2.  Click **Project → Generate Code** (or the “gear” icon in the CubeMX toolbar).
    
3.  Confirm that:
    
    -   `Core/` (user code) is present
        
    -   `Drivers/` and `Middlewares/` are now generated
        

Without this step, the project will not build.


### 3\. Configure WiFi Credentials & Server

Open the WiFi/ESP8266 source file (for example `wifi.c`) and update:  

`#define WIFI_SSID       "your-ssid"`  
`#define WIFI_PASSWORD   "your-password"`  

If you want to send data to your own server instead of the hard-coded one, adjust the IP/URL and HTTP GET string inside the upload function (around `AT+CIPSTART` and the formatted `"GET /...` string).  


### 4\. Wire the Hardware

-   Follow the pin mapping defined in the **`.ioc`** file.
    
-   Ensure:
    
    -   LM35 output goes to the configured ADC pin (e.g. `ADC1_INx`).
        
    -   LCD data and control lines match the GPIOs in the LCD driver.
        
    -   RTC and EEPROM/FRAM are on the configured I²C bus.
        
    -   ESP8266 TX/RX are on the correct UART pins.
        
    -   All modules share **GND** and run at **3.3 V** where required.
        


### 5\. Build & Flash

1.  Connect the **RaayanMini (STM32F401)** board via **ST-Link**.
    
2.  In STM32CubeIDE:
    
    -   Click **Build** to compile.
        
    -   Click **Run** or **Debug** to flash the firmware.
        

On reset, you should see:  

-   LCD splash → RTC check → WiFi init messages.
    
-   Once WiFi connects successfully, LCD will switch to live **time/date/temperature** view.
    
-   RED/GREEN LEDs will start reflecting WiFi status.
    
-   Temperature readings will be periodically uploaded via HTTP GET.
    
-   If WiFi fails, basic logs are written to I²C EEPROM/FRAM instead.
    

* * *

## ⚙️ What This Project Teaches / Demonstrates

-   Using **FreeRTOS (CMSIS-RTOS)** to split functionality into multiple coordinated tasks.
    
-   Interfacing with **LM35 (ADC)**, **RTC + EEPROM (I²C)**, and **ESP8266 (UART AT)** in one design.
    
-   Doing **BCD conversions** and time/date formatting from an external RTC.
    
-   Implementing small but real **state checks** like WiFi link status and a fallback logging path.
    
-   Building a simple but clean **16x2 LCD UI** for embedded systems instead of relying only on UART `printf`.
    
