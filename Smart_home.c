/*
 * DRIVERS.c
 *
 * Created: 12/04/2026 20:58:08
 * Author : aeab2
 */ 

#include "main.h"

typedef enum
{
	PASSWORD_MODE,
	MONITOR_MODE,
	ALARM_MODE
}SystemMode_t;


int main(void)
{
	
	SystemMode_t CurrentMode = PASSWORD_MODE;
	
	DIO_voidSetPinDir(DIO_PORTA,DIO_PIN0, INPUT);
	DIO_voidSetPinDir(LED1_PORT,LED1_PIN,OUTPUT);
	DIO_voidSetPinDir(DIO_PORTB,DIO_PIN0,OUTPUT);
	DIO_voidSetPinDir(DIO_PORTB,DIO_PIN7, INPUT);
	
	ADC_voidInit();
    LCD_voidInit();
	KPD_voidInit();
	u8 pass[4] = {1,2,3,4};
	u8 val[4];
	u8 z = 0;
	float temp = 0;
	u16 result = 0;
	
	DIO_voidSetPinDir(DIO_PORTD, DIO_PIN5, OUTPUT);
	TIM1_voidFastPWMICR(OC1A);
	
	
	TIM1_voidSetOCRval(1000, OC1A); // INITIAL DOOR LOCK
	
    while (1) 
   {
	  u8 tr = 0;
	  LCD_voidSendCommand(1);
	  
	  
	  switch(CurrentMode)
	  {

		  case PASSWORD_MODE:
		  
		  LCD_voidSendCommand(1);
			  
		  while(CurrentMode == PASSWORD_MODE){
	  LCD_voidGotoXY(0,0);
	  LCD_voidSendString("Welcome");
	  LCD_voidGotoXY(0,1);
	  LCD_voidSendString("Enter_Password");
	  
	  
	  for(u8 i = 0; i < 4; i++){
		  
		  z = 0xff;
		  while(z == 0xff){
			  
			  z = KPD_u8GetKey();
			  
		  }
		  
		  val[i] = z;
		  z = 0xff;
		  _delay_ms(200);
	  }	
	  
	  for(u8 i = 0; i < 4; i++){
		  
		  if(val[i] == pass[i]){
			  tr++;
		  }
		  else if(val[i] != pass[i]){
			  LCD_voidSendCommand(1);
			  LCD_voidGotoXY(0,0);
			  LCD_voidSendString("incorrect_pass");
			  _delay_ms(2000);
			  LCD_voidSendCommand(1);
			  break;
			  }
	  }
			
			if(tr == 4){
				 
				 LCD_voidSendCommand(1);
				 LCD_voidGotoXY(0,0);
				 LCD_voidSendString("correct_pass");
				 _delay_ms(2000);
				 TIM1_voidSetOCRval(5100, OC1A);
				 LCD_voidSendCommand(1);
				 LCD_voidGotoXY(0,0);
				 LCD_voidSendString("Door_Opening");
				_delay_ms(5000);
				 TIM1_voidSetOCRval(1000, OC1A);
				 CurrentMode = MONITOR_MODE;
			}
	        else{
				 TIM1_voidSetOCRval(1000, OC1A);
			}
	  }
			break;
			
		    case MONITOR_MODE:
			
			  LCD_voidSendCommand(1);
			  
			while(CurrentMode == MONITOR_MODE)
			{
			result = ADC_u16ReadChannel(ADC0);
			temp = (float)result * 500 / 1023;
			
			if(temp < 50){
			 LCD_voidGotoXY(0,0);
			 LCD_voidSendString("Temp:");
			 LCD_voidGotoXY(6,0);
			 LCD_voidSendNum(temp);
			 LCD_voidGotoXY(0,1);
			 LCD_voidSendString("Status:Normal");
			}
			else if(temp >= 50){
				CurrentMode = ALARM_MODE;
			}
	  }
			break;
			
			case ALARM_MODE:
			
			  LCD_voidSendCommand(1);
			  
			  while(CurrentMode == ALARM_MODE){
				
				DIO_voidSetPinVal(DIO_PORTB,DIO_PIN0,HIGH); 
				_delay_us(500); 
				LCD_voidGotoXY(0,0);
				LCD_voidSendString("Warning");
				LCD_voidGotoXY(0,1);
				LCD_voidSendString("High_Temp!");
				if(DIO_u8ReadPinVal(DIO_PORTB, DIO_PIN7) == 0){CurrentMode = PASSWORD_MODE;}
	 }
	 break;
   }
  }
}




