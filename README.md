# STM32RobotCar
Code for STM32 Robot Car to do line tracking, parking, relay race. This is an integrated project for HKPU EIE3105.

### Demonstrations
|Demonstration  |Github   |YouTube  |
|---------------|---------|---------|   
|1 Basic Control|[Video](https://github.com/SS2867/STM32RobotCar/raw/main/Demo1.mp4)|[Video](https://youtu.be/RcZF5WoT7qE)|
|2 Line Tracking|[Video](https://github.com/SS2867/STM32RobotCar/raw/main/Demo2.mp4)|[Video](https://youtu.be/1vf6ixqQxpQ)|
|3 Car Parking  |[Video](https://github.com/SS2867/STM32RobotCar/raw/main/Demo3.mp4)|[Video](https://youtu.be/u7v_5H8b4sA)|
|4 Relay Race   |[Video](https://github.com/SS2867/STM32RobotCar/raw/main/Demo4.mp4)|[Video](https://youtu.be/sToataau61Y)|
|5 PID Control  |[Video](https://github.com/SS2867/STM32RobotCar/raw/main/Demo5.mp4)|[Video](https://youtu.be/yiMgA1rXB90)|

Presentation [Video](https://youtu.be/6mVgRHxHIKI)

### How to Use
You need to design and build your own car mechanics. As a reference, the car used in all the demonstrations is designed by [OpenJumper](https://www.openjumper.com/home).

Apart from the mechanical design, to reproduce all the demonstrations in this repository, you will also need the following major electronic chip or modules. While some of them are not compulsory and you may choose any substitute according to your own design, it is recommended to use those listed and follow the connection specified in the pin function table if you want to use the code directly. In addition, this is only a list for electronic components.

|Robot Car      |         |Remote Controller |  |
|---------------|---------|-----------|---------|
|STM32(F103RBT6)|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/STM32F103RBT6.png)  |Arduino Uno|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/ArduinoUno.png)|
|USB type B mini cable|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/USB2MiniB.png)  |USB type B cable|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/USB2B.png)|
|Motor Driver Board (or Motor Shield)|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/MotorShield.png)  |(Joystick Shield)|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/JoyStick.png)|
|I/O sensor board|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/IOBoard.png)  | | |
|3.7V Battery x2|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/3V7Battery.png)  |9V Battery|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/9VBattery.png)|
|Bridge Controller (or USB-to-UART adaptor)|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/USB2UART.png) | HC-06 & HC-05 Bluetooth module|![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/HC0605.png)|
|Line tracker module HW-871 (IR emit receiver) |![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/HW871.png) | | |
|Ultrasonic module HC-SR04 |![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/HCSR04.png)| | |
|Speed counter (encoder) |![image](https://raw.githubusercontent.com/SS2867/STM32RobotCar/main/img/Encoder.png)| | |
|Switch, Fuse, Jumper wires| | | |

### Pin Functions in demo 3 & 4:
PB5 (D4) and PA8 (D7): motor direction control

PB11 (TIM2 Ch4) -> PB4 (D5) and PB10 (D6, TIM2 Ch3): motor PWM speed control 

PB0(A3), PA4(A2), PA5(D13), PA9(D8), PA7(D11): 5-way Linetracker (L->R)

PB6 (D10, TIM4 Ch1): R wheel encoder counter

PA6 (D12, TIM3 Ch1) -> PA1(A1), PA0 (A0): Echo and Trigger of HCSR-04 range finder

### Pin Functions in demo 5:
Unless specified, all other connections follows the part `Pin Functions in demo 3 & 4`

PA6 (TIM3 Ch1): L wheel encoder counter
 

### Disclaimer
Note: This is an academic project. If you use (part of) the codes in this repository for academic or commercial purpose, you will be required to **acknowledge** this repository, otherwise it may be considered as **plagarism or IP theft**.
