#include "fsl_device_registers.h"

unsigned short ADC_read16b(void)
{
	ADC0_SC1A = 0x00; //DP0 Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

unsigned short ADC_read16b1(void)
{
	ADC0_SC1A = 0x01; //DP1 Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

unsigned short ADC_read16b2(void)
{
	ADC0_SC1A = 0x13; //DM0 Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

unsigned short ADC_read16b3(void)
{
	ADC0_SC1A = 0x14; //DM1 Write to SC1A to start conversion from ADC_0
	while(ADC0_SC2 & ADC_SC2_ADACT_MASK); // Conversion in progress
	while(!(ADC0_SC1A & ADC_SC1_COCO_MASK)); // Until conversion complete
	return ADC0_RA;
}

int test_function(float y )/// y = adc0 - adc1
{
	if( y >= 0 && y <= 0.3)
	{ return 83;}
	else if ( y >= 0.3 && y < 0.6)
	{ return 83*2;}
	else if ( y >= 0.6 && y < 0.9)
	{ return 83*3;}
	else if ( y >= 0.9 && y < 1.2)
	{ return 83*4;}
	else if ( y >= 1.2 && y < 1.5)
	{ return 83*5;}
	else if ( y >= 1.5 && y < 1.8)
	{ return 83*6;}
	else if ( y >= 1.8 && y < 2.1)
	{ return 83*7;}
	else if ( y >= 2.1 && y < 2.4)
	{ return 83*8;}
	else if ( y >= 2.4 && y < 2.7)
	{ return 83*9;}
	else if ( y >= 2.7 && y < 3.0)
	{ return 83*10;}
	else if ( y >= 3.0 && y < 3.3)
	{ return 83*11;}
}

int p = 450;
int test_function2(float y )/// y = adc2 - adc3
{
	if( y >= 0 && y <= 0.3)
	{ return p;}
	else if ( y >= 0.3 && y < 0.6)
	{ return p*2;}
	else if ( y >= 0.6 && y < 0.9)
	{ return p*3;}
	else if ( y >= 0.9 && y < 1.2)
	{ return p*4;}
	else if ( y >= 1.2 && y < 1.5)
	{ return p*5;}
	else if ( y >= 1.5 && y < 1.8)
	{ return p*6;}
	else if ( y >= 1.8 && y < 2.1)
	{ return p*7;}
	else if ( y >= 2.1 && y < 2.4)
	{ return p*8;}
	else if ( y >= 2.4 && y < 2.7)
	{ return p*9;}
	else if ( y >= 2.7 && y < 3.0)
	{ return p*10;}
	else if ( y >= 3.0 && y < 3.3)
	{ return p*11;}
}

//Function to drive the motor in the direction of the LDRs that get light (Left and Right LDRs)
void PWM_Left_Right(int x)
{
	FTM3_C6SC = 0x1C; // Output-compare; Set output
	// 2 ms high = 5250, left
	//1 ms high 1600, right
	// 1.5 middle 3430
	//1.25 middle right 2515
	//1.75  middle left 4340
	FTM3_C6V = FTM3_CNT + (4030 + x); // high
	while(!(FTM3_C6SC & 0x80))
	GPIOD_PDOR = 0x01;
	FTM3_C6SC &= ~(1 << 7);

	FTM3_C6SC = 0x18; // Output-compare; Clear output
	FTM3_C6V = FTM3_CNT + 48562; // low
	while(!(FTM3_C6SC & 0x80))
	GPIOD_PDOR = 0x00;
	FTM3_C6SC &= ~(1 << 7);
}

int Midp = 3030;

//Function to drive the motor in the direction of the LDRs that get light (Top and Bottom LDRs)
void PWM_Top_Bottom(int x)
{
	FTM3_C6SC = 0x1C; // Output-compare; Set output
	// 2 ms high = 5250, left
	//1 ms high 1600, right
	// 1.5 middle 3430
	//1.25 middle right 2515
	//1.75  middle left 4340
	FTM3_C6V = FTM3_CNT + (Midp + x); // high
	while(!(FTM3_C6SC & 0x80))
	GPIOD_PDOR = 0x02;
	FTM3_C6SC &= ~(1 << 7);

	FTM3_C6SC = 0x18; // Output-compare; Clear output
	FTM3_C6V = FTM3_CNT + 48562; // low
	while(!(FTM3_C6SC & 0x80))
	GPIOD_PDOR = 0x00;
	FTM3_C6SC &= ~(1 << 7);
}

int main(void)
{
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; //Configure Clock Gating for PortD
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK; //Configure Clock Gating for ADC0
	SIM_SCGC3 |= SIM_SCGC3_FTM3_MASK; // FTM3 clock enable

	//Configure Port D Pins 0-1 for GPIO;
	PORTD_GPCLR = 0x00030100;

	// Configure ADC for 16 bits, and to use bus clock.
	ADC0_CFG1 = 0x0C;

	//Disable ADC Module, ADCH = 11111
	ADC0_SC1A = 0x1F;

	//Set PD[7:0] for Output
	GPIOD_PDDR = 0x00000003;

	PORTC_PCR10 = 0x300; // Port C Pin 10 as FTM3_CH6 (ALT3)
	FTM3_MODE = 0x5;     // Enable FTM3
	FTM3_MOD = 0xFFFF;   //Setting Period
	FTM3_SC = 0X0B;      //Setting Prescale = 8

	unsigned short ADC_Value = 0;
	unsigned short ADC_Value1 = 0;
	unsigned short ADC_Value2 = 0;
	unsigned short ADC_Value3 = 0;
	PWM_Left_Right(0);
	float diff = 0;
	int r = 0;
	int d = 0;
	int i = 0;
	int diff2 = 0;
	while (1) {
		//PWM
		// 0 - 3.3,   0 - 1000 -> Map ADC voltages to ticks
		for (i=0; i< 3000; i++);
		ADC_Value = (3.3*ADC_read16b())/65535;    //DP1, Right
		for (i=0; i< 3000; i++);
		ADC_Value1 = (3.3*ADC_read16b1())/65535;  //DP0, Left
		for (i=0; i< 3000; i++);
		ADC_Value2 = (3.3*ADC_read16b2())/65535;  //DM1, Top
		for (i=0; i< 3000; i++);
		ADC_Value3 = (3.3*ADC_read16b3())/65535;  //DM0, Bottom
		for (i=0; i< 3000; i++);
		diff2 = abs(ADC_Value2 - ADC_Value3);
		if (ADC_Value > ADC_Value1)
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75 middle left 4340
			diff = ADC_Value - ADC_Value1;
			while ( diff >= 1.5 &&  diff2 < .5)
			{
				PWM_Left_Right(-83*6);
				PWM_Top_Bottom(0);
				ADC_Value = (3.3*ADC_read16b())/65535;    //DP0, Right
				for (i=0; i< 300; i++);
				ADC_Value1 = (3.3*ADC_read16b1())/65535;  //DP1, Left
				for (i=0; i< 300; i++);
				ADC_Value2 = (3.3*ADC_read16b2())/65535;  //DM1, Top
				for (i=0; i< 300; i++);
				ADC_Value3 = (3.3*ADC_read16b3())/65535;  //DM0, Bottom
				for (i=0; i< 300; i++);
				diff = ADC_Value - ADC_Value1;
				diff2 = abs(ADC_Value2 - ADC_Value3);
			}
			r = -1*test_function(diff);
			PWM_Left_Right(r);
			int diff = 0;
			goto thisjump;
		}
		if (ADC_Value < ADC_Value1)
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75 middle left 4340+/
			// when dp1> dp0 goes to right
			//   dp1< dp0 left
			diff = ADC_Value1 - ADC_Value;
			r = 1*test_function(diff);
			PWM_Left_Right(r);
		}

		else if (ADC_Value == ADC_Value1)
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75  middle left 4340
			PWM_Left_Right(0);
			int diff = 0;

		}
//---------------------------------------------------------------------------------------------------------------------------------------------

		if (ADC_Value2 > ADC_Value3 && ADC_Value1 > ADC_Value )  // dm1 val2 > dm0 ,   val0 dp0
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75  middle left 4340
			diff = ADC_Value2 - ADC_Value3;
			d = -1*test_function2(diff);

			PWM_Top_Bottom(d);
			int diff = 0;
		}
		if (ADC_Value2 > ADC_Value3 && ADC_Value1 < ADC_Value )  // dm0 > dm1 and dp1 < dp0
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75  middle left 4340
			diff = ADC_Value2 - ADC_Value3;
			d = -1*test_function2(diff);
			PWM_Top_Bottom(d);
			int diff = 0;
		}
		if (ADC_Value2 < ADC_Value3 && ADC_Value < ADC_Value1)
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75  middle left 4340
			diff = ADC_Value2 - ADC_Value3;
			d = 1*test_function2(diff);

			PWM_Top_Bottom(d);
			int diff = 0;
		}
		if (ADC_Value2 <  ADC_Value3 && ADC_Value > ADC_Value1)
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75  middle left 4340
			diff = ADC_Value3 - ADC_Value2;
			d = 1*test_function2(diff);
			PWM_Top_Bottom(d);
			int diff = 0;
		}
		if (ADC_Value2 > ADC_Value3 && ADC_Value < ADC_Value1)
		{
			// 2 ms high = 5250, left
			//1 ms high 1600, right
			// 1.5 middle 3430
			//1.25 middle right 2515
			//1.75  middle left 4340
			diff = ADC_Value2 - ADC_Value3;
			d = 1*test_function2(diff);
			PWM_Top_Bottom(d);
			int diff = 0;
		}
	}
	return 0;
}
