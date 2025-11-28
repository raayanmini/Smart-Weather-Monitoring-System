# Smart Weather Monitor System (SWMS) with TFT & DHT11

A weather‐monitoring project built on an STM32F401RBT6 (RaayanMini). It reads temperature and humidity from a DHT11 sensor, displays the data on an ST7735 TFT screen, and sends updates over WiFi using an ESP8266 module. CMSIS-FreeRTOS manages real‐time scheduling so that sensor readings, display updates, and data uploads run smoothly in parallel.  


## 🛠 Tools & Components

- **Microcontroller:** STM32F401RBT6 (RaayanMini)  
- **Sensor:** DHT11 (Temperature & Humidity)  
- **Display:** ST7735 TFT (SPI interface)  
- **WiFi Module:** ESP8266 (AT-command mode over UART)  
- **RTOS:** CMSIS-FreeRTOS (task management, delays, synchronization)  
- **IDE:** STM32CubeIDE v1.18.0    


## 🔧 How It Works

1. **Sensor Reading (FreeRTOS Task)**  
   - A low-priority task wakes every 5 second to read temperature and humidity from the DHT11.

2. **Display Update (FreeRTOS Task)**  
   - A medium-priority task uses SPI to draw the latest readings on the ST7735 TFT.  
   - Because it’s non-blocking, the display never stalls when the WiFi task is busy.

3. **Data Transmission (FreeRTOS Task)**  
   - Every five seconds, a high-priority task formats an HTTP request and sends it via UART to the ESP8266, which forwards the data to a remote server.

4. **RTOS Coordination**  
   - CMSIS-FreeRTOS schedules these three tasks (sensor, display, WiFi) so they run as if parallel—no more blocking `HAL_Delay()` calls. Tasks communicate via semaphores where needed.  

## ⚙️ Features & Highlights

- **Real-Time Scheduling**  
  Sensor reads and display updates happen independently; even if ESP8266 responses lag, the TFT still refreshes smoothly.

- **Multi-Protocol Integration**  
  Combines **SPI** (for the ST7735), **UART** (for the ESP8266), and a custom DHT11 driver—packaged into one firmware image.

- **Wireless Data Upload**  
  ESP8266 in AT-command mode handles HTTP GET/POST requests. Data (temperature + humidity) appears on a remote dashboard every 5 seconds.  
    
## ✔️ Key Takeaways

- **CMSIS-FreeRTOS on STM32F4**  
  Splitting functionality into tasks instead of `HAL_Delay()` makes the system more responsive and reliable.

- **Peripheral Prioritization**  
  Learning to juggle DMA vs. interrupt priorities taught me how to manage multiple interfaces without data collisions.

- **Hands-On Wireless Experience**  
  Crafting AT commands for HTTP requests over UART unlocked a deeper understanding of microcontroller↔WiFi module communication.

- **User Experience**  
  Real-time weather data on a vibrant TFT + online updates is rewarding—especially as a first project under my belt.

## 🔌 Hardware / Pin Mapping

I’m not documenting every single pin in the README because the **exact mapping is already in the `.ioc` file** (CubeMX configuration) in this repo.

Roughly, it uses:

- One GPIO pin for **DHT11 data**  
- **SPI** instance for the ST7735 TFT (SCK, MOSI ; CS, DC, RESET as GPIOs)  
- One **UART** for the ESP8266 (TX/RX)  

If you open the project in STM32CubeIDE and look at the `.ioc` file, you’ll see all the real pin assignments there. That is the source of truth.

## 🚀 Getting Started

### 1\. Clone the Repo

`git clone https://github.com/raayanmini/Smart-Weather-Monitoring-System.git`

`cd Smart-Weather-Monitoring-System`

### 2\. Open in STM32CubeIDE  

1.  Open **STM32CubeIDE v1.18.0**
    
2.  Go to **File → Open Projects from File System…**
    
3.  Point it to this project folder and import.
    
4.  You can inspect/modify pins and middleware from the **`.ioc`** file.
    
### 2\.5\. Generate Code (Drivers + Middleware)  

The repo intentionally does not commit the auto-generated HAL drivers, CMSIS device files, and middleware (FreeRTOS, etc.) – they are ignored via .gitignore and must be regenerated from the .ioc before the first build.  

So after importing the project:  
1. Open the .ioc file in STM32CubeIDE.

2. Click Project → Generate Code (or the “gear” icon in the CubeMX view).

>Confirm that Drivers/ and Middlewares/ folders are now populated.
>If you skip this step, the build will fail because those folders are not in the repo by design.


### 3\. Set Your WiFi / Server Details

In the ESP8266 source file (for example `wifi.c` or similar), update these macros (names may differ slightly):

`#define WIFI_SSID       "your-ssid"`

`#define WIFI_PASSWORD   "your-password"`

`#define SERVER_IP     "yourserverip"`

Right now I’m just sending data every 5 seconds. For a real deployment you’d probably reduce frequency and add more robust error handling.

### 4\. Build & Flash

1.  Connect the **RaayanMini** to your PC through **ST-Link**.
    
2.  In STM32CubeIDE:
    
    -   Click **Build** to compile.
        
    -   Click **Run** (or **Debug**) to flash.

## 📸 Demo

[![Watch the Demo Video](https://img.youtube.com/vi/wjii8i1-mzw/maxresdefault.jpg)](https://www.youtube.com/watch?v=wjii8i1-mzw)

> 🎥 Click the image to watch the Smart Weather Monitor System in action!

