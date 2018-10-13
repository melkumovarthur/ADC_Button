//chipenable.ru   02.10.09
//adc avr- получение первых результатов
//убираем дребезг
//Нужно написать, что бы с переменную tmp(KeyBuf) не присваивали порту В, а делали определённые действия с таймером(добавить, отнять, пуск, стоп)
//и написать функциб для работы с сдвиговым регистром
#define  F_CPU 1200000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#define StartConvAdc()  ADCSRA|=(1<<ADSC)
#define KEY_NULL  0
#define KEY_S1    1
#define KEY_S2    2
#define KEY_S3    3
#define KEY_S4    4
//кнопочный буфер
volatile unsigned char KeyBuf = 0;
unsigned char comp = 0;
unsigned char tmp;
unsigned char timer = 0;

int main( void )
{  
	//настраиваем порты
	DDRB = 0xff;
	DDRB&=~((1<<PB4));
	PORTB = 0x00;
	//инициализируем АЦП
	//ион - напряжение питания, выравнивание влево, второй канал(PB4)
	ADMUX = (0<<REFS0)|(1<<ADLAR)|(1<<MUX1)|(0<<MUX0);
	//вкл. ацп, режим одиночного преобразования, разрешение прерывания, частота преобр. = FCPU/128
	ADCSRA = (1<<ADEN)|(0<<ADSC)|(0<<ADATE)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);	   
	sei();//разрешаем прерывания 
	StartConvAdc();//и запускаем преобразование
	while(1)
	{   tmp = KeyBuf;
		if tmp==4
		{ PORTB|=(1<<3);
		  			
		}
		if tmp==3
		{ PORTB&=~(1<<3);
		}
		if tmp==2
		{ //go timer
			
		}
		if tmp==1
		{
		 //stop timer
		}
		if tmp<1 & tmp>4
		{
		}
	}
	return 0;
}
ISR(ADC_vect)
{
		unsigned char AdcBuf;
		unsigned char Key;
		static unsigned char LastState;		
		//считываем старший регистр АЦП
		AdcBuf = ADCH;		
		//опеределяем в какой диапазон он попал
		if (AdcBuf > 240)
		Key = KEY_S4;
		else if (AdcBuf > 180)
		Key = KEY_S3;
		else if (AdcBuf > 120)
		Key = KEY_S2;
		else if (AdcBuf > 35)
		Key = KEY_S1;
		else
		Key = KEY_NULL;		
		KeyBuf=Key;
		//защита от дребезга и не только. если нажата какая-нибудь кнопка
		//сравниваем совпадают ли текущее состояние и предыдущее
		//если совпадают - проверяем счетчик comp, если нет обнуляем его
		// 		//кнопка считается нажатой если она удерживается в течении 100
		// 		//преобразований АЦП
		if (Key)
		{
		 if (Key == LastState)
		 {if (comp > 100)
		  KeyBuf=Key;
		  else
		  comp++;
		 }
		 else
		 {LastState = Key;
		  comp = 0;
		 }
		}
		else
		{	comp = 0;
			KeyBuf = KEY_NULL;
			LastState = KEY_NULL;
		}
		//запускаем преобразование и выходим
		StartConvAdc();
}
//Нужно написать, что бы с переменную tmp(KeyBuf) не присваивали порту В, а делали определённые действия с таймером(добавить, отнять, пуск, стоп) 
//и написать функциб для работы с сдвиговым регистром



