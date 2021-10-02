/*
 * Division.c
 *
 *  Created on: May 27, 2021
 *      Author: Theocharis Theocharidis
 */
#include <stdio.h>
#include <stdlib.h>
#include "io.h"
#include "alt_types.h"
#include "system.h"


#include "priv/alt_legacy_irq.h"

#define TIMER_STAT_REG 0 // status register address offset
#define TIMER_CTRL_REG 1 // control register address  offset
#define TIMER_PRDL_REG 2 // period reg (lower 16bits) addr offset
#define TIMER_PRDH_REG 3 // period reg (upper 16bits) addr offset


alt_u16 clock_ticks = 0;


void init_timer(alt_u32 timer_base, alt_u32 period)
{
	alt_u16 high,low;

	/* unpack 32-bit timeout period into two 16-bit half words*/
	high = (alt_u16) (period >> 16);
	low = (alt_u16)	(period & 0x0000ffff);

	/* write timeout_period*/
	IOWR(timer_base, TIMER_PRDH_REG, high);
	IOWR(timer_base, TIMER_PRDL_REG, low);

	/* configure timer to start, continuous mode; enable interrupt*/
	IOWR(timer_base, TIMER_CTRL_REG, 0x0007);
}

void timer_0_isr(void* context, alt_u32 id)
{
	//clear timer's status register to (time-out) bit (bit 0) to start the timer over again
	IOWR(TIMER_0_BASE, TIMER_STAT_REG, 0);

	clock_ticks++;
}
int MKD(int x, int y)
{
    int remainder;

     while (1){
    	IOWR(NUMER_PIO_BASE, 0 , x);
    	IOWR(DENOM_PIO_BASE, 0 , y);
    	remainder = IORD(REM_PIO_BASE, 0);
        if (remainder == 0) break;
        x = y;
        y = remainder;
        }
    return (y);
}


int main(){
	//alt_u8 led_pattern = 0; //change = 1; //count = 0;
	int numerator,denominator,quotient,remainder,i,n,d,a,b,init,k,p,difference;

	//IOWR(LED_BASE, 0 , led_pattern);
	init_timer(TIMER_0_BASE, 50000); // init timer with period = 1 msec

	//declare timer_0 isr
	alt_irq_register(TIMER_0_IRQ, NULL, (alt_isr_func)timer_0_isr);

	printf("Insert Numerator:");
	scanf("%d",&numerator);
	printf("Insert Denominator:");
	scanf("%d",&denominator);
	clock_ticks=0;
	init=clock_ticks;

	//1st Execution of the division through the operator '/'
	for (i=0;i<1000000;i++){
	quotient=numerator/denominator;}
	printf("Quotient:%d\n",quotient);
	printf("Remainder:%d\n\n",numerator-(denominator*quotient));
	printf("Initial value clock_ticks: %d\n", init);
	printf("Final Value clock_ticks: %d\n\n", clock_ticks);
	printf("Elapsed %d secs\n\n",(clock_ticks-init)/1000);
	k=clock_ticks-init;
	printf("- - - - - \n\n");

	//2nd Execution of the division through the division circuit 
	printf("***2nd part***\n\n");
	clock_ticks=0;
	init=clock_ticks;
	for (i=0;i<1000000;i++){
		IOWR(NUMER_PIO_BASE, 0 , numerator);
		IOWR(DENOM_PIO_BASE, 0 , denominator);
		quotient = IORD(QUOT_PIO_BASE, 0) ;
		remainder = IORD(REM_PIO_BASE, 0);
	}
	printf("Quotient:%d\n",quotient);
	printf("Remainder:%d\n\n",remainder);
	printf("Initial value clock_ticks: %d\n", init);
	printf("Final Value clock_ticks: %d\n\n", clock_ticks);
	printf("Elapsed %d secs\n\n",(clock_ticks-init)/1000);
	p=clock_ticks-init;
    difference=k-p;
	printf(" The division through the division circuit is by %d secs faster than division using the operator '/'.\n\n",difference/1000);



	while (1){
		printf("\n*** 3rd Part ***\n");
		printf("*** Greatest Common Divisor (GCD) calculation using Euclid's algorithm ***\n\n");
		printf("Insert 1st Integer:");
		scanf("%d",&a);
		printf("Insert 2nd Integer:");
		scanf("%d",&b);
		n=abs(a);//Absolute value of the 1st integer 
		d=abs(b);//Absolute value of the 2nd integer 



		if ((a==0)||(b==0))//Control for the introduction of integers with zero values 
		{
			if (a==0)
			{
				if (b>0)
				{
					printf(" The GCD of %d and of %d is %d.\n", a, b, b);
				}
				else if (b<0)
				{
					printf("The GCD of %d and of %d is %d.\n", a, b, d);
				}
				else if (b==0)
				{
					printf("The GCD of %d and of %d is %d.\n", a, b, 0);
				}
			}
			else if (b==0)
			{
				if (a>0)
				{
					printf("The GCD of %d and of %d is %d.\n", a, b, a);
				}
				else if (a<0)
				{
					printf("The GCD of %d and of %d is %d.\n", a, b, n);
				}
			}
		}
		if ((a<0)||(b<0)){//Control for the introduction of integers with negative values 
			if (a<0){
				if (b>0)
				{
					printf("The GCD of %d and of %d is %d.\n", a, b, (MKD(n, b)));
				}
				else if (b<0)
				{
					printf("The GCD of %d and of %d is %d.\n", a, b, (MKD(n,d)));
				}
			}
			else if (b<0){
				if (a>0){
					printf("The GCD of %d and of %d is %d.\n", a, b, (MKD(a,d)));
				}
			}
		}
		if (a>0){//Control for the introduction of integers with positive values 
			if (b>0){
				printf("The GCD of %d and of %d is %d.\n", a, b, MKD(a,b));
			}
		}

	}
}
