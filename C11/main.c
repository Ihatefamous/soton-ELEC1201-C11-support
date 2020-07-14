//Ihatefamous, under Creative Commons Attribution License as well


#include "lcd.h"
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <stdlib.h>


void rectangle_diff(rectangle new,rectangle old,unsigned int newcolor){
	if(new.left<0||new.right>LCDWIDTH||new.top<0||new.bottom>LCDHEIGHT)
		return ;

	if(new.left>old.right||new.top>old.bottom||new.right<old.left||new.bottom<old.top||old.left<0||old.right>LCDWIDTH||old.top<0||old.bottom>LCDHEIGHT){
		fill_rectangle(old,BLACK);
		fill_rectangle(new,newcolor);
		return ;
	}

	if (new.left==old.left){

	} else if (new.left>old.left){
		rectangle temp={old.left,new.left-1,old.top,old.bottom};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={new.left,old.left-1,new.top,new.bottom};
		fill_rectangle(temp,newcolor);
	}

	if (new.right==old.right){

	} else if (new.right<old.right){
		rectangle temp={new.right+1,old.right,old.top,old.bottom};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={old.right+1,new.right,new.top,new.bottom};
		fill_rectangle(temp,newcolor);
	}

	if(new.top==old.top){

	} else if (new.top>old.top){
		rectangle temp={old.left,old.right,old.top,new.top-1};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={new.left,new.right,new.top,old.top-1};
		fill_rectangle(temp,newcolor);
	}

	if(new.bottom==old.bottom){

	} else if (new.bottom<old.bottom){
		rectangle temp={old.left,old.right,new.bottom+1,old.bottom};
		fill_rectangle(temp,BLACK);
	} else {
		rectangle temp={new.left,new.right,old.bottom+1,new.bottom};
		fill_rectangle(temp,newcolor);
	}
}

#define platex 100
#define platey 5
#define platemove 3
#define ball_size 8



volatile int newx=(LCDWIDTH-platex)/2,newy=0;//position of the plate
volatile int ball_newx=-1,ball_newy=-1;//position of the ball
volatile int detect;
volatile _Bool finish=0;

ISR(TIMER1_OVF_vect){
	static int savex = -1, savey=-1;
	static int ball_savex=-1,ball_savey=-1;
	rectangle a = {newx,newx+platex,newy,newy+platey},b = {savex,savex+platex,savey,savey+platey};
	rectangle c = {ball_newx,ball_newx+ball_size,ball_newy,ball_newy+ball_size},d = {ball_savex,ball_savex+ball_size,ball_savey,ball_savey+ball_size};
	rectangle_diff(a,b,YES);
	rectangle_diff(c,d,GREEN);
	savex=newx;
	savey=newy;
	ball_savex=ball_newx;
	ball_savey=ball_newy;
}

ISR(TIMER0_OVF_vect){
	static unsigned int wait=0;
	static int ballmovex=1, ballmovey=1;
	if(wait<100){
		wait++;
	} else if (wait==100){
		srand(detect);
		ball_newx = rand()%LCDWIDTH;
		ball_newy = rand()%LCDHEIGHT;
		wait++;
	} else {
		ball_newx+=ballmovex;
		ball_newy+=ballmovey;

	if(ball_newx<0){
		ballmovex=-ballmovex;
		ball_newx=0;
	} else if((ball_newx+ball_size)>LCDWIDTH){
		ballmovex=-ballmovex;
		ball_newx=LCDWIDTH-ball_size;
	}
	if(ball_newy<0){
		ballmovey=-ballmovey;
		ball_newy=0;
		finish=1;
	} else if((ball_newy+ball_size)>LCDHEIGHT){
		ballmovey=-ballmovey;
		ball_newy=LCDHEIGHT-ball_size;
	}
	if(ball_newx<(newx+platex)&&(ball_newx+ball_size)>newx&&ball_newy==(newy+platey+1)){
		ballmovey=-ballmovey;
	}
	}
}

const int match[] = {0,1,3,2};

volatile int save;

ISR(PCINT3_vect){
	int now = PIND & ( _BV(0) | _BV(1) );
	switch((match[now]-match[save])){
	case 0: break;
	case 1:case -3: newx+=platemove;break;
	case -1:case 3: newx-=platemove;break;
	}
	if(newx<0){
		newx=0;
	}else if(newx+platex>LCDWIDTH){
			newx=LCDWIDTH-platex;
	}
	save = now;
	detect++;
}

int main(){
	init_lcd();
	set_orientation(0);
	DDRD &= ~(_BV(0) | _BV(1));
	PORTD |= _BV(0) | _BV(1);
	TCCR1B  |= _BV(CS10);
	TCCR0B	|= _BV(CS02);
	TIMSK1  |= _BV(TOIE1);
	TIMSK0  |= _BV(TOIE0);
	set_sleep_mode (SLEEP_MODE_PWR_DOWN);
	save = PIND & ( _BV(0) | _BV(1) );
	PCMSK3 =_BV(0) | _BV(1);
	PCICR |= _BV(PCIE3);
	sei();
	while(!finish){
		sleep_cpu();
	}
	return 0;
}
