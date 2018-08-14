/*

	Demo of glcd library with AVR8 microcontroller
	
	Tested on a custom made PCB (intended for another project)

	See ../README.md for connection details

*/

#include <avr/io.h>
#include "glcd/glcd.h"
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include "glcd/fonts/Liberation_Sans15x21_Numbers.h"
#include "glcd/fonts/font5x7.h"
#include <avr/pgmspace.h>
#include <util/delay.h>



#define F_CPU 16000000UL  // 1 MHz
#define gruener_Taster !(PIND&(1<<PD5))				//Definition f�r Abfrage des gr�nen Taster
#define roter_Taster !(PIND&(1<<PD6))				//Definition f�r Abfrage des roten Tasters
#define blauer_Taster !(PIND&(1<<PD2))				//Definition f�r Abfrage des blauen Tasters
#define blauer_Taster_Ruhezustand PIND&(1<<PD2)		//Definition f�r Abfrage des blauen Tasters im Ruhezustand


/* Function prototypes */
static void setup(void);

static void setup(void)
{
	/* Set up glcd, also sets up SPI and relevent GPIO pins */
	glcd_init();
}

uint8_t ms, ms10,ms100,sec,min,entprell, state;
uint16_t Ball_X;					//Initialisierung f�r Ball
uint16_t Ball_Y;					//Initialisierung f�r Ball
uint16_t Schrift_X;					//Initialisierung f�r Schrift
uint16_t Schrift_Y;					//Initialisierung f�r Schrift
uint16_t Balken_unten_X;			//Initialisierung f�r Balken unten
uint16_t Balken_unten_Y;			//Initialisierung f�r Balken unten
uint16_t Balken_oben_X;				//Initialisierung f�r Balken oben
uint16_t Balken_oben_Y;				//Initialisierung f�r Balken oben
uint16_t Balken_weiss_unten_X;		//Initialisierung f�r weisser Balken unten
uint16_t Balken_weiss_unten_Y;		//Initialisierung f�r weisser Balken unten
uint16_t Balken_weiss_oben_X;		//Initialisierung f�r weisser Balken oben
uint16_t Balken_weiss_oben_Y;		//Initialisierung f�r weisser Balken oben
uint8_t Lauflicht;					//Initialisierung f�r Ballrichtung
char String[3]={000};				//Initialisierung f�r String mit 10 Feldern
uint8_t Variable=123;				//Initialisierung der Variable
uint8_t Nichts;						//Initialisierung f�r Nichts, sodass "Variable=123" durch dr�cken des blauen Tasters gel�scht wird


ISR(TIMER0_OVF_vect)
{
	static uint8_t ISR_zaehler=0;
	static uint8_t ms100=0;
	static uint8_t sek=0;
	static uint8_t min=0;
	static uint8_t h=0;
	static uint8_t d=0;
	static uint8_t w=0;
	
	
	TCNT0 = 0;					
	ISR_zaehler++;					//"ISR_zaehler" aufw�rts z�hlen
	if(ISR_zaehler==12)				//Sobald 8bit zw�lf mal durchlaufen (100ms), Z�hler wieder auf Null setzen
	{
		ISR_zaehler=0;				//Auf Null zur�cksetzen sobald 100ms erreicht sind
		ms100++;
		Schrift_X++;						//"ms100" aufw�rts z�hlen
	}		

	
	if(ms100==10)					//Sobald 8bit 12-mal durchlaufen (1sek), Z�hler wieder auf Null setzen
	{
		ms100=0;					//Auf Null zur�cksetzen sobald 1sek erreicht sind
		sek++;						//"sek" aufw�rts z�hlen
	}
	
	
	if(sek==60)						//Sobald 8bit 60-mal durchlaufen (1min), Z�hler wieder auf Null setzen
	{
		sek=0;						//Auf Null zur�cksetzen sobald 1min erreicht sind
		min++;						//"min" aufw�rts z�hlen
	}
	
	
	if(min==60)						//Sobald 8bit 60-mal durchlaufen (1h), Z�hler wieder auf Null setzen
	{
		min=0;						//Auf Null zur�cksetzen sobald 1h erreicht sind
		h++;						//"h" aufw�rts z�hlen
	}
	
	
	if(h==24)						//Sobald 8bit 24-mal durchlaufen (1d), Z�hler wieder auf Null setzen
	{
		h=0;						//Auf Null zur�cksetzen sobald 1d erreicht sind
		d++;						//"d" aufw�rts z�hlen
	}

	
	if(d==7)						//Sobald 8bit 7-mal durchlaufen (1w), Z�hler wieder auf Null setzen
	{
		d=0;						//Auf Null zur�cksetzen sobald 1w erreicht sind
		w++;						//"w" aufw�rts z�hlen
		Ball_Y++;
	}							
	
}									//End ISR




const unsigned char batman[] PROGMEM= 
{ 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x80, 0xc0, 0xc0, 0xe0, 0xe0, 0xf0, 0xf0, 0xf8, 
	0xf8, 0xf8, 0xfc, 0xfc, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 
	0xff, 0xe7, 0x83, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x80, 0xc0, 0xfe, 0xfc, 0xf8, 0xf8, 0xfc, 0xfe, 0xc0, 
	0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x83, 
	0xe7, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfc, 0xfc, 
	0xf8, 0xf8, 0xf8, 0xf0, 0xf0, 0xe0, 0xe0, 0xc0, 0xc0, 0x80, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xc0, 0xe0, 
	0x70, 0x78, 0x3c, 0x3e, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 
	0x7f, 0x7f, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 
	0xfe, 0xfe, 0xfe, 0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xfe, 0xfe, 0xfe, 0xfe, 
	0xfe, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0x7f, 0x7f, 
	0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3f, 0x3e, 0x3c, 0x78, 0x70, 
	0xe0, 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x0f, 
	0x3f, 0x7f, 0x3f, 0x3f, 0X1f, 0x1f, 0x0f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0xff, 0xff, 0xff, 
	0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 
	0xff, 0x7f, 0x3f, 0x3f, 0x1f, 0x1f, 0x1f, 0x0f, 0x0f, 0x0f, 
	0x0f, 0x1f, 0x1f, 0x1f, 0x3f, 0x3f, 0x7f, 0x3f, 0x0f, 0x03, 
	0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x01, 0x03, 0x07, 0x0f, 0x3f, 0xff, 0xff, 0x3f, 
	0x0f, 0x07, 0x03, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
	0x00, 0x00, 0x00, 0x00
	};
	
int main(void)
{
	DDRD |=(1 << PD2);		//Taster_2 als EIngang setzen
	PORTD |=(1 << PD2);	//Ausgang Taster_2 auf low setzen (PULLUP-WIDERSTAND)
	DDRD |=(1 << PD5);		//Taster_5 als EIngang setzen
	PORTD |=(1 << PD5);	//Ausgang Taster_5 auf low setzen (PULLUP-WIDERSTAND)
	DDRD |=(1 << PD6);		//Taster_6 als EIngang setzen
	PORTD |=(1 << PD6);	//Ausgang Taster_6 auf low setzen (PULLUP-WIDERSTAND)
	
	TCCR0A		= 0x00;
	TCCR0B		= 0x04;
	TIMSK0		|= (1 << TOIE0);
	TIFR0	|= (1 << TOV0);
	sei();
	
	/* Backlight pin PL3, set as output, set high for 100% output */
	DDRB |= (1<<PB2);
	PORTB |= (1<<PB2);
	

    sei();					// enable interrupts
	
	setup();
	
	glcd_clear();
	glcd_write();
	
	Ball_X=42;
	Ball_Y=25;
	Schrift_X=0;
	Schrift_Y=0;
	Balken_unten_X=34;
	Balken_unten_Y=46;
	Balken_oben_X=34;
	Balken_oben_Y=8;
	Balken_weiss_unten_X=Balken_unten_X-1;
	Balken_weiss_unten_Y=Balken_unten_Y;
		
	// Display
	glcd_tiny_set_font(Font5x7,5,7,32,127);
	glcd_clear_buffer();
		
	
	while(1) 
	{
		if(Lauflicht==0)										//Sobald Ball gleich 0 ist, soll Ball_Y aufw�rts z�hlen und somit Ball nach unten fallen
		{
			Ball_Y++;
		}
		if(Lauflicht==1)										//Sobald Ball gleich 1 ist, soll Ball_Y abw�rts gez�hlt und somit Ball nach oben gehen
		{
			Ball_Y--;
		}
		glcd_draw_string_xy( Schrift_X,  Schrift_Y,  "HALLO");				//Schrift (�bertitel)
		glcd_fill_rect(Schrift_X-1,Schrift_Y,1,7,WHITE);						//Weisser Balken nach Schrift
		glcd_fill_rect(Balken_unten_X,Balken_unten_Y,16,2,BLACK);						//Schwarzer Balken (unten) um Ball abprallen zu k�nnen
		glcd_fill_rect(Balken_oben_X,Balken_oben_Y,16,2,BLACK);						//Schwarzer Balken (oben) um Ball abprallen zu k�nnen
		glcd_fill_circle( Ball_X, Ball_Y-1 , 2, WHITE);				
		glcd_fill_circle( Ball_X, Ball_Y, 2, BLACK);
		glcd_fill_circle( Ball_X, Ball_Y+1 , 2, WHITE);
		glcd_fill_circle( Ball_X, Ball_Y, 2, BLACK);

		if(Ball_Y==44)												//Sobald der Ball den Balken erreichte, soll der Wert wieder auf Anfang gesetzt werden (unten)
		{
			Lauflicht=1;
		}
		if(Ball_Y==14)												//Sobald der Ball den Balken erreichte, soll der Wert wieder auf Anfang gesetzt werden (oben)
		{
			Lauflicht=0;
		}
		if(Schrift_X==85)												//Sobald die Schrift am Ende des Bildschirms angelangt ist, soll die Schrift auf der anderen Seite wieder eingef�gt werden
		{
			Schrift_X=-40;
		}
		if(gruener_Taster)
		{
			glcd_fill_rect(Balken_weiss_unten_X++,Balken_weiss_unten_Y,1,2,WHITE);
			glcd_fill_rect(Balken_unten_X++,Balken_unten_Y,1,2,BLACK);
		}
		if(roter_Taster)
		{
			glcd_fill_rect(Balken_weiss_unten_X--,Balken_weiss_unten_Y,1,2,WHITE);
			glcd_fill_rect(Balken_unten_X--,Balken_unten_Y,1,2,BLACK);
		}
		if(blauer_Taster)
		{
			sprintf(String, "%d", Variable);
			glcd_draw_string_xy(0,15,String);
		}
		if(blauer_Taster_Ruhezustand)
		{
			sprintf(String, "000%d",Nichts);
			glcd_draw_string_xy(0,15,String);
		}
		glcd_write();
	}//End of while
	
	
	
	//---------------------------------------------
	
	return 0;
}//end of main
