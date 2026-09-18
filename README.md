# Drawing Tablet

<img width="727" height="490" alt="image" src="https://github.com/user-attachments/assets/468cf2be-40ba-4a82-8f87-abc4abec723e" />

I am making a bluetooth drawing tablet that can send images you make on it to your computer. The main audience is people who love digital drawing but cant afford proper hardware like a Ipad and apple pencil or Large drawing pad. 


$\huge\text{Features}$
____________________________________________________________________________________________________________________________________________________________________________________________________________________________
- High quality image export (via BLE or Wifi)
- Multicolor drawing
- Save pictures to ESP32 memory
- Long battery life (3-4 hour, with continuous usage )
- Built in Stylus storage
- Large Screen (2.8 inches)







$\huge\text{Schematic}$
____________________________________________________________________________________________________________________________________________________________________________________________________________________________
<img width="1062" height="661" alt="image" src="https://github.com/user-attachments/assets/1680861e-45cf-4c0b-bb15-c413c26f5891" />
The schematic is easy to understand and has neat wiring




$\huge\text{PCB Design }$
____________________________________________________________________________________________________________________________________________________________________________________________________________________________

<img width="477" height="740" alt="image" src="https://github.com/user-attachments/assets/40b435be-52f2-4358-994c-d0a7ee76f2ee" />

$\huge\text{BOM }$
____________________________________________________________________________________________________________________________________________________________________________________________________________________________



$\large\text{Price in INR }$


| Ref.       | Qty | Part / Value             | LCSC #        |      Price / pc |
| ---------- | --: | ------------------------ | ------------- | --------------: |
| BT2        |   1 | JST-PH 2-pin             | **C265016**   |           ₹6.41 |
| C1, C4     |   2 | 4.7 µF                   | **C915849**   |          ₹56.46 |
| C5         |   1 | 10 µF                    | **C111896**   |          ₹78.10 |
| C6         |   1 | 0.1 µF                   | **C106148**   |          ₹11.42 |
| C7, C8     |   2 | 1 µF                     | **C91185**    |           ₹3.55 |
| D1, D2, D3 |   3 | Indicator LED            | **C125084**   |           ₹3.95 |
| IC1        |   1 | NCP186 3.3 V LDO         | **C719823**   |          ₹32.75 |
| J1         |   1 | 1×14 female socket       | **C2905420**  |          ₹14.69 |
| J2         |   1 | 1×5 male header          | **C55629812** |           ₹3.69 |
| J3         |   1 | USB-C USB 2.0 receptacle | **C3025063**  |          ₹98.75 |
| R1, R2, R6 |   3 | 470 Ω                    | **C114564**   |           ₹0.51 |
| R3, R4     |   2 | 10 kΩ                    | **C84376**    |           ₹0.63 |
| R5, R10    |   2 | 5.1 kΩ                   | **C728389**   |          ₹2.14  |
| R7         |   1 | 1.5 kΩ                   | **C309587**   |           ₹1.52 |
| R8, R9     |   2 | 10 kΩ                    | **C84376**    |           ₹0.63 |
| SW3, SW4   |   2 | Push button              | **C318884**   |           ₹1.98 |
| U1         |   1 | ESP32-S3-MINI-1-N4       | **C2847294**  |           ₹ 369 |
| U4         |   1 | USB ESD protection       | **C94934**    |          ₹51.26 |
| U5         |   1 | MCP73871                 | **C144351**   |         ₹456.94 |


$\huge\text{CREDITS }$
____________________________________________________________________________________________________________________________________________________________________________________________________________________________


FUSION 360 - https://www.autodesk.com/in/products/fusion-360/overview

KICAD - https://www.kicad.org/

$\large\text{FIRMWARE CREDITS  }$

SPI.h

TFT_eSPI.h  By Bodmer 

Bluetoothserial.h



