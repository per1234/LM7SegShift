/*************************************************************************
**  Device: 7-Segment with Shift register                               **
**  File:   LM_7SegShift.cpp   				          	                **
**	Version: 0.1                                                        **
**									                                    **
**  Created by Leonardo Machado 2012-11-15                              **
**									                                    **
**  Based on Code from:							                        **
**    http://bildr.org/2011/08/74hc595-breakout-arduino/ 			    **
**    and                                                               **
**    A. Hinkel http://www.alhin.de/arduino  			                **
**									                                    **
**  The code is provided under the MIT license please use, edit,        **
**  change, and share.  		                    		            **
**                                                                  	**
**************************************************************************/

#include <Arduino.h>
#include <LM7SegShift.h>

//************************************************************************
// Constructor
// Set pins for shift register, how many digits the led have and
// how many shift registers are in use
LM7SegShift::LM7SegShift(int SER_Pin, int RCLK_Pin, int SRCLK_Pin, int qtyShift){
	_SERIAL_DATA_Pin          = SER_Pin == 0 ? 11 : SER_Pin;
	_REGISTER_CLOCK_Pin       = RCLK_Pin == 0 ? 12 : RCLK_Pin;
	_SHIFT_REGISTER_CLOCK_Pin = SRCLK_Pin == 0 ? 13 : SRCLK_Pin;
	_qtyShift = qtyShift;
	_numOfRegisterPins = 8 * _qtyShift;

	pinMode(_SERIAL_DATA_Pin, OUTPUT);
	pinMode(_REGISTER_CLOCK_Pin, OUTPUT);
	pinMode(_SHIFT_REGISTER_CLOCK_Pin, OUTPUT);

	clear(); 
}

const byte LM7SegShift::seven_seg_digits[10][7] = { { 0,0,0,0,0,0,1 },  // = 0
													{ 1,0,0,1,1,1,1 },  // = 1
													{ 0,0,1,0,0,1,0 },  // = 2
													{ 0,0,0,0,1,1,0 },  // = 3
													{ 1,0,0,1,1,0,0 },  // = 4
													{ 0,1,0,0,1,0,0 },  // = 5
													{ 0,1,0,0,0,0,0 },  // = 6
													{ 0,0,0,1,1,1,1 },  // = 7
													{ 0,0,0,0,0,0,0 },  // = 8
													{ 0,0,0,1,1,0,0 }   // = 9
												  };

//************************************************************************
// defineSegments
// Define the pin in shift register for each segment and if the led
// is Common Anode or Common Cathode
void LM7SegShift::defineSegments(int segA, int segB, int segC, int segD, int segE, int segF, int segG, int segDot, int isAnode){
	_segA = segA;
	_segB = segB;
	_segC = segC;
	_segD = segD;
	_segE = segE;
	_segF = segF;
	_segG = segG;
	_segDot = segDot;

	_isAnode = isAnode;

	pin_order[0] = _segA;
	pin_order[1] = _segB;
	pin_order[2] = _segC;
	pin_order[3] = _segD;
	pin_order[4] = _segE;
	pin_order[5] = _segF;
	pin_order[6] = _segG;
}

//************************************************************************
// defineDigits
// Define the pin in shift register for each segment digit
void LM7SegShift::defineDigits(int digits, int dig1, int dig2, int dig3, int dig4){
	_digits = digits;

	digit_order[0] = dig1;
	digit_order[1] = dig2;
	digit_order[2] = dig3;
	digit_order[3] = dig4;

	int digPos = _digits - 1;

	for(int x = 0; x < _digits; x++)
	{
		for(int y = 0; y < _digits; y++)
		{
			_digitos[x][y] = y == digPos ? 1 : 0;
		}
		digPos--; 
	}
}

//************************************************************************
// clear()
// Clear all pin status.
// If the LED in use is a Common Anode then the HIGH means turned off
// If the LED in use is a Common Cathode then the LOW means turned off
void LM7SegShift::clear(){
    // Turn off all segments of the 7-Segment
	for(int i = _numOfRegisterPins - 1; i >=  0; i--){
		_registers[i] = _isAnode ? HIGH : LOW;
	}

	// Discover the last pin used by the 7-Segmnent and clear the
	// pin status until the end of all pins of the 595
	int lastPin = 8 + _digits;

	for(int i = lastPin; i < _numOfRegisterPins; i++){
		_registers[i] = LOW;
	}
}

void LM7SegShift::reset(){
	clear();
	writeRegisters();
}

void LM7SegShift::writeRegisters(){
	digitalWrite(_REGISTER_CLOCK_Pin, LOW);

	for(int i = _numOfRegisterPins - 1; i >=  0; i--){
		digitalWrite(_SHIFT_REGISTER_CLOCK_Pin, LOW);
		digitalWrite(_SERIAL_DATA_Pin, _registers[i]);
		digitalWrite(_SHIFT_REGISTER_CLOCK_Pin, HIGH);
	}

	digitalWrite(_REGISTER_CLOCK_Pin, HIGH);
}

void LM7SegShift::setRegisterPin(int index, int value){
	byte finalValue = value;
	if (!_isAnode)
		finalValue = value == 1 ? 0 : 1; 
	_registers[index] = finalValue;
}

void LM7SegShift::setRegisterPinExtras(int index, int value){
	_registers[index] = value;
	writeRegisters();
}

void LM7SegShift::pickDigit(int number)
{
	int pinseq = 0;

	for (byte digCount = 0; digCount < _digits; ++digCount) 
	{
		setRegisterPin(digit_order[pinseq], _digitos[number-1][digCount]);
		++pinseq;
	}
}

void LM7SegShift::digitWrite(byte digit, byte number) {
	if (digit > _digits)
		return;

	int pinseq = 0;

	pickDigit(digit);

	for (byte segCount = 0; segCount < 7; ++segCount) 
	{
		setRegisterPin(pin_order[pinseq], seven_seg_digits[number][segCount]);
		++pinseq;
	}

    // Turn off the DOT
	setRegisterPin(_segDot, _isAnode ? HIGH : LOW); 

	// Agora força que os registradores sejam enviados para o LED
	writeRegisters();

	delay(5);  // DISPLAY_BRIGHTNESS
}

void LM7SegShift::valueWrite(int number) {
	if (number < 10)
	{
		digitWrite(1, number);
		digitWrite(2, 0);
		digitWrite(3, 0);
		digitWrite(4, 0);
	}
	else if (number < 100)
	{
		digitWrite(1, number % 10);
		digitWrite(2, number / 10);
		digitWrite(3, 0);
		digitWrite(4, 0);
	}
	else if (number < 1000)
	{
		digitWrite(1, number % 10);
		digitWrite(2, (number % 100) / 10);
		digitWrite(3, number / 100);
		digitWrite(4, 0);
	}
	else
	{
		digitWrite(1, number % 10);
		digitWrite(2, (number % 100) / 10);
		digitWrite(3, (number % 1000) / 100);
		digitWrite(4, number / 1000);
	}
}
