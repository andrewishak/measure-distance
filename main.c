#include "tm4c123gh6pm.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////
#define GPIO_PORTA_DATA_R       (*((volatile unsigned long *)0x400043FC))
#define GPIO_PORTA_DIR_R        (*((volatile unsigned long *)0x40004400))
#define GPIO_PORTA_AFSEL_R      (*((volatile unsigned long *)0x40004420))
#define GPIO_PORTA_DEN_R        (*((volatile unsigned long *)0x4000451C))
#define GPIO_PORTA_AMSEL_R      (*((volatile unsigned long *)0x40004528))
#define GPIO_PORTA_PCTL_R       (*((volatile unsigned long *)0x4000452C))
#define PA2_5                   (*((volatile unsigned long *)0x400040F0))
#define PA6_7                   (*((volatile unsigned long *)0x40004300))
///////////////////////////////////////////////////////////////////////////
///////////////////////////////////// port b
#define PB2_5                   (*((volatile unsigned long *)0x400050F0))
#define PB6                     (*((volatile unsigned long *)0x40005100))
#define PB7                     (*((volatile unsigned long *)0x40005200))
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
/////////////////////////////////////////////////////////////////////////
////////////////////////////////////// clock 
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108))
#define NVIC_ACTLR_R            (*((volatile unsigned long *)0xE000E008))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
 ///////////////////////////////////////////////////////////////////////////////////
unsigned long time,dis,x,y,z;

void SysTick_Init(void)
{
NVIC_ST_CTRL_R=0;
NVIC_ST_RELOAD_R=0x00FFFFFF;
NVIC_ST_CURRENT_R=0;
NVIC_ST_CTRL_R=0x00000005;
}
void SysTick_wait( int delay)
{
	
NVIC_ST_RELOAD_R=delay-1;
NVIC_ST_CURRENT_R=0;
while((NVIC_ST_CTRL_R&0x00010000)==0){};
}


void PortA_Init(void){ volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x03;           // 1) activate clock for Port A
  delay = SYSCTL_RCGC2_R;           // allow time for clock to start
                                    // 2) no need to unlock PA2
  GPIO_PORTA_PCTL_R &= 0x00000000; // 3) regular GPIO
  GPIO_PORTA_AMSEL_R &= 0x00;      // 4) disable analog function on PA2
  GPIO_PORTA_DIR_R |= 0xFC;         // 5) set direction to output
  GPIO_PORTA_AFSEL_R |= 0x00;      // 6) regular port function
  GPIO_PORTA_DEN_R |= 0xFC;         // 7) enable digital port
}
void PortB_Init(void){ volatile unsigned long delay;
  GPIO_PORTB_PCTL_R &= 0x00000000; // 3) regular GPIO
  GPIO_PORTB_AMSEL_R &= 0x00;      // 4) disable analog function on PA2
  GPIO_PORTB_DIR_R |= 0x7C;         // 5) set direction to output
  GPIO_PORTB_AFSEL_R |= 0x00;      // 6) regular port function
  GPIO_PORTB_DEN_R |= 0xFC;         // 7) enable digital port
}

void num2seg(void){
x=(dis%10);
	y=((dis%100)-x);
	if(y!=0)
		y=y/10;
	z=dis/100;
	
}
void delayMicro(unsigned long Micro)
{
SysTick_wait((16*Micro));
}
void measure(void){
    PB6=0X00;                  // TRIGER LOW
	  SysTick_wait(160);          //delay 10Micro
		PB6=0x40;                // triger high
		    SysTick_wait(160);
    PB6=0X00;                // TRIGER LOW
		
	   while((PB7&0x80) ==0){;}
			//intialize timer
		 			SysTick_Init();
	while((PB7&0x80) !=0){;}
			time =NVIC_ST_RELOAD_R-NVIC_ST_CURRENT_R;
			dis=((time*.017)/16);
	}




	int main(void){
  PortA_Init();             
  PortB_Init();   
  SysTick_Init();	
  while(1){
		measure();
		num2seg();
		PA2_5=(x<<2);
		PB2_5=(y<<2);
		PA6_7=(z<<6);
	    delayMicro(1000000);
		  delayMicro(1000000);
	// 	delayMicro(1000000);
	
 }
}
