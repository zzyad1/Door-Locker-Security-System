# Door-Locker-Security-System
# Overview
This project implements a Door Locker Security System using two ATmega32 Microcontrollers. The system secures access to a door by requiring a user-defined password, and it operates based on a layered architecture model. The project includes a Human-Machine Interface (HMI) for user interaction and a Control unit that handles all the decision-making processes.

# Features
* Password Management: Users can set and change passwords using a 4x4 keypad, with feedback displayed on a 2x16 LCD screen.
* Door Control: The system verifies the password and controls a DC motor to lock or unlock the door.
* Security Measures: After three failed password attempts, the system triggers a buzzer alarm and locks out further inputs for a specified time.
* EEPROM Storage: The system stores the password securely in an EEPROM for persistent access.

# Technical Specifications
* Microcontrollers: 2x ATmega32 running at 8 MHz.
* Drivers: GPIO, LCD, Keypad, DC Motor, EEPROM, I2C, UART, Timer, and Buzzer.
* Communication: UART is used for communication between the HMI ECU and Control ECU.
* Motor Control: The DC motor operates in PWM mode to control the door locking mechanism.
* Timer Functions: Timers manage the duration of motor operation and display messages.

# System Sequence
* Password Creation: Users are prompted to create a 5-digit password, which is then stored in EEPROM.
* Main Menu: Users can choose to open the door or change the password.
* Door Operation: The system unlocks or locks the door based on the correct password entry.
* Security Lockout: After three incorrect attempts, the system locks and activates an alarm.

# Installation
* Connect the ATmega32 microcontrollers as per the circuit diagram.
* Upload the provided code to the microcontrollers.
* Power the system and follow the on-screen instructions on the LCD.
