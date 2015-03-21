/********************************************************************************
PPMdecode.h - Library for PPM decoding.
Created by Thomas D. , 2015.
With special thanks to Eddy Caron.
Released into the public domain.
********************************************************************************/

#ifndef PPMdecode_h
#define PPMdecode_h
#include "Arduino.h"

#define maxChannel 20

/********************************************************************************
Class
********************************************************************************/

class PPMdecode
{
public:
	PPMdecode(short pin, short channels);
	boolean synchronized;
	void PWMstore();
	long channel[maxChannel];

private:
	short _pin, _channels;
	void makeReference();
	void init();
	boolean state;
	long nowMs, lastMs, diffMs;
	short currentChannel;
};

/********************************************************************************
Static
********************************************************************************/

//Please, dont use in your arduino sketch, this is static.
//copy of instances (needed for interrupts)

static PPMdecode(*dr[99]); //Don't modify directly
static void rerefer();
static short reference = 0;

#endif
