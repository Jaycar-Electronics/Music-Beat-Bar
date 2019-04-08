
#define LOG_OUT 1 // LOG output
#define FHT_N 128 // 128 samples

#include <FHT.h> // FFT-like library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>  //display
#include <Adafruit_NeoPixel.h> //LEDs

// we are representing the whole strip as one unit, 32 pixels long
Adafruit_NeoPixel strip = Adafruit_NeoPixel(32, 4, NEO_GRB + NEO_KHZ800); //GREEN RED BLUE
// 128 x 64 display
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup()
{
	//atmel registers
	TIMSK0 = 0; // turn off timer0 for lower jitter
	ADCSRA = 0xe5; // set the adc to free running mode
	ADMUX = 0x40; // use adc0
	DIDR0 = 0x01; // turn off the digital input for adc0

	display.begin(SSD1306_SWITCHCAPVCC,0x3C);
	display.clearDisplay();
	strip.begin();
	strip.setBrightness(20); //turn up or down as required
	strip.show(); // all off by default
}

void loop()
{
	//this is faster than relying on the loop() calls
	while(1)
	{

		cli();  //clear interrupts

		for (int i = 0 ; i < FHT_N ; i++)
		{

			while(!(ADCSRA & 0x10))
			; // wait for adc to be ready

			ADCSRA = 0xf5; // restart adc

			byte m = ADCL; // fetch adc data
			byte j = ADCH;
			int k = (j << 8) | m; // form into an int

			k -= 0x0200; // form into a signed int
			k <<= 6; // form into a 16b signed int

			fht_input[i] = k; // fill out array
		}
		fht_window();	// window the data for better frequency response
		fht_reorder();	// reorder the data before doing the fht
		fht_run();		// process the data in the fht
		fht_mag_log();	// take the output of the fht

		sei();

		//get ready to display data:
		display.clearDisplay();

		int col_height[4] = {3,3,3,3};

		//skip the first two because they are super low freq.
		for(int i = 2; i < FHT_N/2; i++)
		{
			//clear out anything that isn't better than 32
			if (fht_log_out[i] < 32)
			{
				fht_log_out[i] = 0;
			}
			else
			{
				fht_log_out[i] -= 32; //reduce by 32 just to get a clearer image
			}

			fht_log_out[i] *= 2; //double the strength of it (*2)

			j = i*2;

			//divide by 4 to get value between 1-64;
			display.drawRect(i*2, 0, 2,fht_log_out[i]/4, WHITE);

			//for the below specific frequency bins, we'll put a value in one of the
			//row displays. this is to represent the value at that particular
			//height.

			//divide by 32 to get value between 1-8;
			//we only have 8 pixels per row
			if (i == 8) 	col_height[0] = fht_log_out[i] / 32;
			if (i == 12)	col_height[1] = fht_log_out[i] / 32;
			if (i == 16)	col_height[2] = fht_log_out[i] / 32;
			if (i == 20)	col_height[3] = fht_log_out[i] / 32;

		}
		//finally draw to display
		display.display();

		//for each pixel, figure out row, and if it should be coloured or not.
		for(int i = 0; i < 32; i++)
		{
			int col = (i/8); // column index between 0-3

			if((col+1)%2) //odd / even columns
			{
				if ( col_height[col] < 7-(i%8))
					strip.setPixelColor(i, strip.Color(0,0,0));
				else
					strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(i*2048)); //stolen from examples
			}
			else
			{
				if ( col_height[col] < i%8)
					strip.setPixelColor(i, strip.Color(0,0,0));
				else
					strip.setPixelColor(i, strip.gamma32(strip.ColorHSV(i*2048+256)); //stolen from examples
			}
		}
		//once all the pixels have been set, turn the strip on, and continue loop
		strip.show();
	}
}
// use this function to return a colour in a rainbow sort of pattern.
// for the moment we will just return a single colour, but
uint32_t colourWheel(int16_t count, int offset){
	strip.
}
