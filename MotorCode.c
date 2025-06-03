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
	- Create a Motor init function and include Systick Init inside it ( PA7 for outpur)
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
#define GPIO_PORTA_DR8R					(*((volatile uint32_t *)0x40004508) //Offset 0x508
	

//ALL ABOUT NVIC (ctrl, reload, pri, current )
#define NVIC_PRI0_R  						(*((volatile uint32_t *)0xE000E400)) //PRI0 because interrupt 0 ( Port A ) ( OFFSET 0X400)
#define NVIC_PRI3_R  						(*((volatile uint32_t *)0xE000E40C)) //PRI3 because SysTick interrupt ( OFFSET 0X40C)
#define NVIC_STCTRL_R						(*((volatile uint32_t *)0xE000E010)) //Offset 0x010
#define NVIC_STRELOAD_R					(*((volatile uint32_t *)0xE000E014)) //Offset 0x014
#define NVIC_STCURRECNT_R				(*((volatile uint32_t *)0xE000E018)) //Offset 0x018

	
//**GENERAL VARIABLE**//

uint32_t High; //For high PWM
uint32_t Low;  //For low PWM


//**FUNCTIONS**//

// Enable global interrupts
void EnableInterrupts(void) {
    __asm("CPSIE I");  // CPSIE I = Clear Interrupt Disable bit, enabling interrupts
}

// Wait for interrupt 
void WaitForInterrupts(void) {
    __asm("WFI");  // WFI = Wait For Interrupt instruction
}

void GPIO_Init (void)
{
	SYSCTL_RCGCGPIO_R |= 0x01; // Turn On Clock Port A
	GPIO_PORTA_DEN_R |= 0xB0;  //1011 0000, PA4 and PA5 and PA7 Enable
	GPIO_PORTA_DIR_R |= 0x80; //1000 0000,PA4 & PA5 Input, PA7 Output 
	GPIO_PORTA_DATA_R &= ~0x80; // 0111 1111, neglect everyone so PA7 is low
	GPIO_PORTA_PUR_R |= 0x30; //0011 0000, PA4 & PA5 Pull up enabled	

}

void DCMotor_Init(void)
{
	High = Low = 1600; //10% from 16000 ( 16000 cycles, so 1 ms )
	NVIC_STCTRL_R = 0;
	NVIC_STRELOAD_R = Low - 1; //reload per 10%
	NVIC_STCURRECNT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R & 0x00FFFFFF) | 0x60000000; // setting to priority 3 ( under switch's priority) ( Using PRI3 as per ARM Manual not TM4CNCPDT Datasheet remember)
	NVIC_STCTRL_R = 0x07; //bit 0-2 on
	
}
