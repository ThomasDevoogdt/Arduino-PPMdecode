/********************************************************************************
PPMdecode.h - Library for PPM decoding.
Created by Thomas D. , 2015.
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
	init();
}
void PPMdecode::init()
{
	for (short i = 0; i < _channels; i++)
	{
		channel[i] = initValue;
	}
	synchronized = false;
	lastMs = 0;
	makeReference();
	attachInterrupt(_pin, rerefer, RISING);
}
void PPMdecode::makeReference()
{
	dr[reference] = this;
	reference++;
}

/********************************************************************************
Interrupt function
********************************************************************************/

static void rerefer(){
	for (short i = 0; i < reference; i++){
		(*dr[i]).PWMstore();
	}
}

void PPMdecode::PWMstore(){
	nowMs = micros();
	diffMs = nowMs - lastMs;
	if ((lastMs > 0) && (diffMs > initTime)){
		currentChannel = 0;
		synchronized = true;

	}
	else  if (synchronized && (currentChannel <= _channels) && (diffMs > minPuls) && (diffMs < maxPuls))
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