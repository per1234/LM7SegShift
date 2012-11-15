#include <LM7SegShift.h>

int SER_Pin = 11;   
int RCLK_Pin = 12;  
int SRCLK_Pin = 13; 

int digitQty = 2;
int currentValue = 0;
int currentMillis = 0;

LM7SegShift disp(SER_Pin, RCLK_Pin, SRCLK_Pin, 2); 

void setup()  { 
  Serial.begin(9600);

  //----------------------------------------------------------------------------------------
  // Define the pin in Shift Register for each one of the 7 segments
  // The last parameter if a boolean value indicating if the Led is anode or cathode
  // Definition
  //    defineSegments(segA, segB, segC, segD, segE, segF, segG, segDot, isAnode);
  disp.defineSegments(0, 1, 2, 3, 4, 5, 6, 7, 1);

  //----------------------------------------------------------------------------------------
  // Define the pin in Shift Register for each one of the digits of the 7 segment LED
  // The first parameter is the quantity of pins you LED have
  // For now up to 4 digits are allowed. Pass 0 for the ones that you dont have
  // Definition
  //    defineDigits(digits, dig1, dig2, dig3, dig4)
  disp.defineDigits(digitQty, 8, 9, 0, 0);
} 

void loop()  { 
   if (Serial.available())
      currentValue = Serial.parseInt();

  disp.valueWrite(currentValue);
  
  //----------------------------------------------------------------------------------------
  // For each digit 5 ms are spent to show, so if you multiply the quantity of 
  // digits for 5 you have the amount of time ellapsed in the call
  currentMillis = currentMillis + (5 * digitQty);

  if (currentMillis > 1000)
  {
    currentValue++;
    currentMillis = 0;
  }
}

