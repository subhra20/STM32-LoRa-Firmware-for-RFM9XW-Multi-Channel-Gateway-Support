# STM32-LoRa-Firmware-for-RFM9XW-Multi-Channel-Gateway-Support
Firmware for integrating the RFM9XW LoRa module with STM32 using HAL. Includes reliable transmit and receive functions, along with a 4-channel LoRa gateway implementation for parallel reception—ideal for multi-channel IoT applications. Modular and easy to extend for custom STM32-based LoRa projects.

# 📡 STM32 LoRa Communication Library

A lightweight, fully configurable LoRa (HopeRF RFM95/96/97/98(W) ) library for STM32 microcontrollers using HAL. Easily set up robust wireless communication for low-power, long-range applications. Supports multi-channel FreeRTOS-based gateways and basic LoRa nodes.

---

## ✨ Features

- ✅ Full access to LoRa parameters:
  - Bandwidth, Spreading Factor, Carrier Frequency, Coding Rate, Transmission Power
- 🔌 Overcurrent protection
- 📤 Transmit and 📥 Receive data
- 📶 RSSI estimation (signal strength)
- 🧠 Easy-to-use API

---

## ✅ Successfully Tested On

- STM32F407 Discovery Board  
- STM32 Nucleo F429ZI  
- 7Semi STM32G030F6P6 Anchor STM32

---

## 🔧 Hardware Requirements

- [RFM9xW LoRa Module](https://www.hoperf.com/modules/lora/RFM95.html)
- [RFM9xW Breakout PCB (Assembled)](https://github.com/attexx/rfm9x_breakout_board)
- GSM Antenna – 2 dBi Rubber Duck R/A SMA (M)
- STM32 Microcontroller (Any board with SPI and EXTI support)

---

## ⚙️ Wiring

| STM32 Pin               | LoRa Module Pin        |
|------------------------|------------------------|
| SPI MISO               | MISO                   |
| SPI MOSI               | MOSI                   |
| SPI CLK                | CLK                    |
| GPIO output (HIGH)     | NSS                    |
| GPIO output (HIGH)     | RST                    |
| GPIO input (EXTI)      | DIO0                   |
| 3.3V                   | 3.3V                   |
| GND                    | GND                    |

---

# Installation
Download or copy [LoRa.c](https://github.com/SMotlaq/LoRa/blob/master/LoRa/LoRa.c) and [LoRa.h](https://github.com/SMotlaq/LoRa/blob/master/LoRa/LoRa.h)
, import them into your project and then include LoRa.h in main.c:
```C
#include "LoRa.h"
```
# Initial configurations
First of all, you'll have to create a LoRa object:
```C
 LoRa myLoRa;
```
Then you'll have to call its constructor in your main function:
```C
 myLoRa = newLoRa();
```
Now, the default settings are set in ```myLoRa```, but you must set 7 important parameters:

 * NSS pin
 * NSS port
 * Reset pin
 * Reset port
 * DIO0 port
 * DIO0 pin
 * The SPI used for communication

```C
myLoRa.CS_port         = NSS_GPIO_Port;
myLoRa.CS_pin          = NSS_Pin;
myLoRa.reset_port      = RESET_GPIO_Port;
myLoRa.reset_pin       = RESET_Pin;
myLoRa.DIO0_port       = DIO0_GPIO_Port;
myLoRa.DIO0_pin        = DIO0_Pin;
myLoRa.hSPIx           = &hspi3;
```

Now, calling ```LoRa_init()```, will write these settings in the module's memory.
```C
LoRa_init(&myLoRa);
```
This function returns a "status code" to represent module response status. 
The status codes are:
* ```200```: LORA_OK - Everything is OK.
* ```404```: LORA_NOT_FOUND - Your microcontroller can't communicate with the LoRa module and read the RegVersion.
* ```503```: LORA_UNAVAILABLE - Something in LoRa's settings (i.e. NSS port/pin, RESET port/pin or SPI handler) is not correct.

Other status codes:
* ```413```: LORA_LARGE_PAYLOAD - Your data is larger than 255 bytes.
### Example
```C
#include "stdio.h"
#include "string.h"
.
.
.
char   send_data[200];
uint16_t LoRa_status = LoRa_init(&myLoRa);
memset(send_data,NULL,200);

if (LoRa_status==LORA_OK){
  snprintf(send_data,sizeof(send_data),"\n\r LoRa is running... :) \n\r");
  LoRa_transmit(&myLoRa, (uint8_t*)send_data, 120, 100);
  HAL_UART_Transmit(&debugUART, (uint8_t*)send_data, 200, 200);
}
else{
  snprintf(send_data,sizeof(send_data),"\n\r LoRa failed :( \n\r Error code: %d \n\r", LoRa_status);
  HAL_UART_Transmit(&debugUART, (uint8_t*)send_data, 200, 200);
}
```
Other parameters are set like the following:
```C
myLoRa.frequency             = 434;             // default = 433 MHz
myLoRa.spredingFactor        = SF_9;            // default = SF_7
myLoRa.bandWidth             = BW_250KHz;       // default = BW_125KHz
myLoRa.crcRate               = CR_4_8;          // default = CR_4_5
myLoRa.power                 = POWER_17db;      // default = 20db
myLoRa.overCurrentProtection = 130;             // default = 100 mA
myLoRa.preamble              = 10;              // default = 8;
```
### Spreading factor values
```C
//--- SPREADING FACTORS ---//
SF_7      7
SF_8      8
SF_9      9
SF_10     10
SF_11     11
SF_12     12
```
### Bandwidth values
```C
//------- BANDWIDTH -------//
BW_7_8KHz     7.8   KHz
BW_10_4KHz    10.4  KHz
BW_15_6KHz    15.6  KHz
BW_20_8KHz    20.8  KHz
BW_31_25KHz   31.25 KHz
BW_41_7KHz    41.7  KHz
BW_62_5KHz    62.5  KHz
BW_125KHz     125   KHz
BW_250KHz     250   KHz
BW_500KHz     500   KHz
```
### Coding rate values
```C
//------ CODING RATE ------//
CR_4_5    4/5
CR_4_6    4/6
CR_4_7    4/7
CR_4_8    4/8
```
### Power values
```C
//------ POWER GAIN ------//
POWER_11db  11db
POWER_14db  14db
POWER_17db  17db
POWER_20db  20db
```
### Over current protection
The maximum current must be a multiple of 5 if it is less than 120, and a multiple of 10 if it is greater than 120.
The minimum value is 45 mA and the maximum is 240 mA. See [this file](https://github.com/SMotlaq/LoRa_third/blob/master/OCP_values.xlsx?raw=true) to learn about allowed currents.

# Transmitting Data
After these configurations, now you can transmit a set of bytes using the LoRa_transmit function:

```uint8_t LoRa_transmit(LoRa* _LoRa, uint8_t* data, uint8_t length, uint16_t timeout)```

**NOTE: After calling this function, the FiFo data buffer will be cleared. It means that all received packets will be deleted. If you want to read data just after transmitting something, you should add a delay.**
### Arguments:
* ```_LoRa```: Your LoRa object
* ```data```: A pointer to the data you wanna send
* ```length```: Size of your data in Bytes
* ```timeOut```: Timeout in milliseconds
### Returns:
* ```1``` in case of success, ```0``` in case of timeout

### Example:
1. 
```C
char*  send_data;
send_data = "Hello world!";
LoRa_transmit(&myLoRa, (uint8_t*)send_data, 12, 100);
```  
2.
```C
char*  send_data;
send_data = "Hello world!";
if(LoRa_transmit(&myLoRa, (uint8_t*)send_data, 12, 100) == 1){
   HAL_GPIO_WritePin(LED_GPIO_Port, LED_Pin, GPIO_PIN_SET);
}
```  
# Receive Data
LoRa modules have two receive modes:
1. Single
2. Continuous

This library only supports "Continuous mode".

First of all, you'll have to enable receiving data:
```C
LoRa_startReceiving(&myLoRa);
```
This function changes operating mode from STANDBY to RXCONTINUOUS, and after that you can store the last received packet in a variable by calling ```LoRa_receive```.  You can call ```LoRa_receive``` in a timer interrupt callback (recommended) or in your main loop.

```uint8_t LoRa_receive(LoRa* _LoRa, uint8_t* data, uint8_t length)```
### Arguments:
* ```_LoRa```: Your LoRa object
* ```data```: A pointer to the array that you want to write received bytes in.
* ```length```: The number of bytes you want to read.

### Returns:
* The size of recieved packet in bytes.

### Example:
```C
LoRa_startReceiving(&myLoRa);

uint8_t received_data[10];
uint8_t packet_size = 0;
while(1){
  packet_size = LoRa_receive(&myLoRa, received_data, 10);
  Hal_Delay(500);
}
```
# Signal power estimation
The SX127x series can measure the power of the last received packet. The ```LoRa_getRSSI(...)``` can do this.

```int LoRa_getRSSI(LoRa* _LoRa)```
### Arguments:
* ```_LoRa```: Your LoRa object
### Returns:
* An integer representing the power in dBm. For example -43.
