# COE 718 — Lab 1: Joystick Control

Implementation of a joystick-driven interface using the MCB1700 development board to control LEDs and a graphical LCD.

**Status:** Implementation in progress

---

## Objectives
The goal of this lab is to familiarize students with the Keil µVision IDE, ARM Cortex-M3 architecture, and peripheral interfacing. The system reads input from a onboard joystick and provides visual feedback via 8 user LEDs and an SPI-based GLCD.

---

## Specifications

| Parameter | Value |
| :--- | :--- |
| **MCU** | ARM Cortex-M3 (LPC1768) |
| **Board** | MCB1700 |
| **Joystick Pins** | P1.20, P1.23, P1.24, P1.25, P1.26 (GPIO1) |
| **LEDs** | 8 User LEDs |
| **Display** | SPI GLCD (Optional via `#define __USE_LCD`) |
| **Toolchain** | Keil µVision |
| **Tick Rate** | 10 ms (SysTick) |

---

## Project Structure

```text
Lab 1/
├── Boards/
│   └── Keil/MCB1700/Blinky_ULp/  # Keil Project files (.uvprojx, etc.)
├── Blinky.c                     # Main application logic
├── Blinky.h                     # Global definitions
├── IRQ.c                         # SysTick handler (10ms timing)
├── KBD.c / KBD.h                 # Joystick driver
├── LED.c / LED.h                 # LED driver
├── GLCD_SPI_LPC1700.c / GLCD.h   # LCD driver
├── Font_6x8_h.h / Font_16x24_h.h # LCD Fonts
└── LAB 1 - Introduction...pdf    # Lab Handout
```

---

## How it Works

The system operates using a periodic timer interrupt (SysTick) and a main polling loop:

1.  **Timing**: The `SysTick_Handler` triggers every 10ms. It maintains a `clock_ms` flag used for periodic tasks (like debug printing) and manages LED blinking patterns based on system state.
2.  **Input**: The `KBD_get()` and `get_button()` functions monitor the GPIO pins connected to the joystick. The joystick returns a bitmask identifying the current direction (Up, Down, Left, Right, or Select).
3.  **Output**:
    *   **LEDs**: Based on the detected joystick direction, the `LED_Out()` function updates the state of the 8 user LEDs to visually represent the input.
    *   **LCD**: If `__USE_LCD` is defined, the system initializes the SPI GLCD and displays the current direction and a header.
4.  **Debug**: The system utilizes the ITM (Instrumentation Trace Macrocell) for `printf` output via the SWO pin, allowing real-time debugging of variable values without halting the CPU.

---

## Build & Run

1.  Open `Lab 1/Boards/Keil/MCB1700/Blinky_ULp/Blinky.uvprojx` in **Keil µVision**.
2.  Select the **MCB1700** target device.
3.  Build the project to generate the `.axf` binary.
4.  Flash the binary to the LPC1768 via SWD/J-Link.
5.  (Optional) Configure the **Debug → Settings → Trace** menu to enable SWO/ITM printf output to the console.

---

## Assignment

With the code given to you in this tutorial, and the joystick (KBD.*) files and notes found in the 
Appendix of this lab or Use Joystick from Device Run time environment, create a program which will 
read the direction that the joystick is pressed on the MCB1700 dev board. Based on the direction of the 
joystick, the following peripherals should function as follows:

• LCD - will display the last direction that the joystick has been tilted/pressed. Design a suitable 
header and title on your LCD for demo purposes. 
• LEDs - depending on the direction the joystick has been last tilted/pressed, an LED(s) will turn 
on representing the direction. Examine LED.c from the tutorial to understand how LEDs are 
turned on and off. 

Have an option which enables or disables the LCD like the blinky tutorial. Disable the LCD when in 
Debug mode. Analyze your program (your TA may ask you questions regarding simulation during your 
demo).

This lab is due in week 3 at the beginning of your lab session. You are expected to deliver the following:

1. Print the source code for your lab  including the main files, and any .h or .c files provided to 
you during the tutorial that you may have altered for your application. Ensure that you include 
the Ryerson University title page, dated and signed, with your code attached. 

2. Present a demo displaying your joystick implementation, and the output to the LCD and LEDs 
on the dev board. You may also be quizzed on your understanding of your joystick 
implementation when in Debug mode. Be prepared.
