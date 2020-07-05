#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>


void rectangle_diff(rectangle new,rectangle old){
	if(new.left>old.right||new.top>old.bottom||new.right<old.left||new.bottom<old.top){
		fill_rectangle(old,BLACK);
		fill_rectangle(new,YES);
		return ;
	}
	if (new.left==old.left){

	} else if (new.left>old.left){
		rectangle temp={old.left,new.left-1,old.top,old.bottom};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={new.left,old.left-1,new.top,new.bottom};
		fill_rectangle(temp,YES);
	}

	if (new.right==old.right){

	} else if (new.right<old.right){
		rectangle temp={new.right+1,old.right,old.top,old.bottom};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={old.right+1,new.right,new.top,new.bottom};
		fill_rectangle(temp,YES);
	}

	if(new.top==old.top){

	} else if (new.top>old.top){
		rectangle temp={old.left,old.right,old.top,new.top-1};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={new.left,new.right,new.top,old.top-1};
		fill_rectangle(temp,YES);
	}

	if(new.bottom==old.bottom){

	} else if (new.bottom<old.bottom){
		rectangle temp={old.left,old.right,new.bottom+1,old.bottom};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={new.left,new.right,old.bottom+1,new.bottom};
		fill_rectangle(temp,YES);
	}
}

#define width 9

volatile int num=0;

ISR(TIMER1_OVF_vect){
	static int save = 0;
	rectangle a = {0,LCDWIDTH,num,num+width},b = {0,LCDWIDTH,save,save+width};
	rectangle_diff(a,b);
	save=num;
}

const int match[] = {0,1,3,2};

volatile int save/* = PIND & ( _BV(0) | _BV(1) )*/;

/*ISR(TIMER0_OVF_vect){
	int now = PIND & ( _BV(0) | _BV(1) );
	switch((match[now]-match[save])){
	case 0: break;
	case 1:case -3: num++;break;
	case -1:case 3: num--;break;
	default: num=0;
	}
	save = now;
}*/


ISR(PCINT3_vect){
	int now = PIND & ( _BV(0) | _BV(1) );
	switch((match[now]-match[save])){
	case 0: break;
	case 1:case -3: num+=3;break;
	case -1:case 3: num-=3;break;
	//default: num=0;
	}
	if(num<0)
		num=0;
	if(num+width>LCDHEIGHT)
		num=LCDHEIGHT-width;
	save = now;
}

int main(){
	init_lcd();
	set_orientation(0);
	DDRD &= ~(_BV(0) | _BV(1));
	PORTD |= _BV(0) | _BV(1);
	TCCR1B  |= /*_BV(CS11) |*/ _BV(CS10);
	TIMSK1  |= _BV(TOIE1);
	//TCCR0B  |= /*_BV(CS02) | */_BV(CS00);
	//TIMSK0  |= _BV(TOIE0);
	set_sleep_mode (SLEEP_MODE_PWR_DOWN);
	save = PIND & ( _BV(0) | _BV(1) );
	PCMSK3 =_BV(0) | _BV(1);
	PCICR |= _BV(PCIE3);
	rectangle temp = {0,LCDWIDTH,num,num+width};
	fill_rectangle(temp,YES);
	sei();
	while(1){
		sleep_cpu();
	}
	return 0;
}
