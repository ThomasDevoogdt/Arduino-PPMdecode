/********************************************************************************
PPMdecode.h - Library for PPM decoding.
Created by Thomas D. & David D. , 2015.
With special thanks to Eddy Caron.
Released into the public domain.
********************************************************************************/

#include "Arduino.h"
#include "PPMdecode.h"

#define minPuls 1000 //µs
#define maxPuls 2000 //µs
#define initTime 5000 //µs
#define initValue 50 //%

/********************************************************************************
Setup
********************************************************************************/

PPMdecode::PPMdecode(short pin, short channels)
{
	_pin = pin;
	_channels = channels;

	for (short i = 0; i < _channels; i++)
	{
		channel[i] = initValue;
	}
	lastMs = 0;

	dr[reference] = this;
	attachInterrupt(_pin, voidList[reference], RISING);
	reference++;
}

void PPMdecode::PWMstore(){
	nowMs = micros();
	diffMs = nowMs - lastMs;
	if ((lastMs > 0) && (diffMs > initTime)){
		currentChannel = 0;
		synchronized = true;

	}
	else  if (synchronized && (currentChannel < (_channels + 1)) && (diffMs > minPuls) && (diffMs < maxPuls))
	{
		channel[currentChannel] = map(diffMs, minPuls, maxPuls, 0, 100);
		currentChannel++;
	}
	if (currentChannel == (_channels + 1))
	{
		currentChannel = 0;
		synchronized = false;
	}
	lastMs = nowMs;
}

/********************************************************************************
Interrupt function
********************************************************************************/
static void update0(){ (*dr[0]).PWMstore(); }//I hate doing this, but the attachInterrupt needs this.
static void update1(){ (*dr[1]).PWMstore(); }
static void update2(){ (*dr[2]).PWMstore(); }
static void update3(){ (*dr[3]).PWMstore(); }
static void update4(){ (*dr[4]).PWMstore(); }
static void update5(){ (*dr[5]).PWMstore(); }

