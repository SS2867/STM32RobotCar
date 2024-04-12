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
