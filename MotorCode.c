//LAB DESIGN
/*

1. Plan the Hardware specification
	- Diode
	- Voltage Regulator
	- Resistors 
	- DC Motor 3 - 6v
	- Battery ( Lit Ion 3 (3.7 volt each)) and Battery holder for power source
	- Capacitors 10uF
	- Transistor
	
2. Try to create a DC motor works as instruction
	- We will make the DC motor works using PWM ( High and low phase ), using SysTick interrupts for automatically spinning and using switch to set the speed based on PWM
	- Using 16 MHz base frequency and PORT A as control output from MCU
	- We will make systick per ms, 16.000.000 / 1000 = 16000 cycles per 1 ms. So 10% of it should be 1600 for L.
	- Based on this, we are creating algorithm like these ( 1600, 3200, 4800, 6400, ......, 16000) per 10% of the base frequency
	- When switch 1 is pressed, Speed increased, when switch 2 is pressed, speed decreased ( per 10%)
	

3. Software Specification
	- Create a Motor init function and include Systick Init inside it
	- Create systick handler ( how it performs constantly so you dont have to always press the button to make DC Motor spins)
	- Create Switch init ( init the pull up and edge interrupt)
	- Create switch handler ( the logic when the switches are pressed)
	- Create main funct

*/


#include <stdint.h>


//**ADDRESS**//

//PORT A GPIO
#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608)) //RCGCGPIO Address
#define GPIO_PORTA_BASE					(*((volatile uint32_t *)0x40004000)) //Base address for Port A
#define GPIO_PORTA_DATA_R       (*((volatile uint32_t *)0x400043FC)) //Offset 0x3fc
#define GPIO_PORTA_DIR_R        (*((volatile uint32_t *)0x40004400)) //Offset 0x400
#define GPIO_PORTA_PUR_R        (*((volatile uint32_t *)0x40004510)) //Offset 0x510
#define GPIO_PORTA_DEN_R        (*((volatile uint32_t *)0x4000451C)) //Offset 0x51c
	

//ALL ABOUT NVIC (ctrl, reload, pri, current )
#define NVIC_PRI0_R  						(*((volatile uint32_t *)0xE000E400)) //PRI0 because interrupt 0 ( Port A ) ( OFFSET 0X400)
#define NVIC_STCTRL_R						(*((volatile uint32_t *)0xE000E010)) //Offset 0x010
#define NVIC_STRELOAD_R					(*((volatile uint32_t *)0xE000E014)) //Offset 0x014
#define NVIC_STCURRENT_R				(*((volatile uint32_t *)0xE000E018)) //Offset 0x018

//**FUNCTIONS**//

// Enable global interrupts
void EnableInterrupts(void) {
    __asm("CPSIE I");  // CPSIE I = Clear Interrupt Disable bit, enabling interrupts
}

// Wait for interrupt 
void WaitForInterrupts(void) {
    __asm("WFI");  // WFI = Wait For Interrupt instruction
}

void MotorInit (void)
{
	
}