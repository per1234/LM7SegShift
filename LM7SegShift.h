/*************************************************************************
**  Device: 7-Segment with Shift register                               **
**  File:   LM_7SegShift.h                                              **
**	Version: 0.1                                                        **
**									                                    **
**  Created by Leonardo Machado 2012-11-15                              **
**									                                    **
**  Based on Code from:                                                 **
**    http://bildr.org/2011/08/74hc595-breakout-arduino/ 			    **
**    and                                                               **
**    A. Hinkel http://www.alhin.de/arduino  			                **
**									                                    **
**  The code is provided under the MIT license please use, edit,        **
**  change, and share.  		                    		            **
**                                                                  	**
**************************************************************************/

#ifndef LM7SegShift_h
#define LM7SegShift_h

#include <Arduino.h>   //Arduino IDE >= V1.0

class LM7SegShift
{
public:
	// Constructor
	LM7SegShift(int SER_Pin, int RCLK_Pin, int SRCLK_Pin, int qtyShift);

	//Register functions

	// Clear all registers from the ShiftRegister
	// PS: Just clear. Dont flush to the register. To flush call writeRegisters()
	void clear();

	// Setup the segments used in the ShiftRegister, and which type is the Led
	void defineSegments(int segA, int segB, int segC, int segD, int segE, int segF, int segG, int segDot, int isAnode);
	
	// Setup each digit used in the ShiftRegister
	void defineDigits(int digits, int dig1, int dig2, int dig3, int dig4);

	// Write a number to a specific digit in the 7-Segment
	void digitWrite(byte digit, byte number);

	// Clear and flush to the shift register. This function combines the clear() and the writeRegisters()
	void reset();

	// Set a specific pin in the shift register. Flush immediatelly
	void setRegisterPinExtras(int index, int value);

	// Write a full number to the 7-Segment led
	void valueWrite(int number);

	// Flush the registers to the Shift Register
	void writeRegisters();
private:
	int  _SERIAL_DATA_Pin;
	int  _REGISTER_CLOCK_Pin;
	int  _SHIFT_REGISTER_CLOCK_Pin;
	byte _SHIFT_REGISTER;
	int _isAnode;
	int _qtyShift;
	int _numOfRegisterPins;

	int _digits;
	int _segA;
	int _segB;
	int _segC;
	int _segD;
	int _segE;
	int _segF;
	int _segG;
	int _segDot;

	static const byte seven_seg_digits[10][7];
	byte pin_order[7];
	byte digit_order[4];
	byte _digitos[4][4];
	byte _registers[16];

	void setRegisterPin(int index, int value);
	void pickDigit(int number);
};

#endif 
