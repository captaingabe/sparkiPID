/*******************************************
 Sparki Line-following example
 
 Threshold is the value that helps you 
 determine what's black and white. Sparki's 
 infrared reflectance sensors indicate white 
 as close to 900, and black as around 200.
 This example uses a threshold of 500 for 
 the example, but if you have a narrow line, 
 or perhaps a lighter black, you may need to 
 adjust.
********************************************/

#include <Sparki.h> // include the sparki library

#define BASESPEED 100
#define PGAIN 1
#define DGAIN .7
#define IGAIN .1

int speedLeft;
int speedRight;

int diffIntgrl = 0;

void setup() 
{
  speedLeft = BASESPEED;
  speedRight = BASESPEED;
}

void loop() {
  
  int lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int lineRight  = sparki.lineRight();  // measure the right IR sensor

  int lineDiff = (int)((float)(lineRight - lineLeft)); // calculate cross track error
  
  delay(100);
  
  int final_lineLeft   = sparki.lineLeft();   // measure the left IR sensor
  int final_lineCenter = sparki.lineCenter(); // measure the center IR sensor
  int final_lineRight  = sparki.lineRight();  // measure the right IR sensor
  
  int final_lineDiff = (int)((float)(final_lineRight - final_lineLeft)); // calculate second cross track error after .1 sec

  int diffDeriv = (int)((float)(final_lineDiff - lineDiff)); // take derivative of cross track error

  diffIntgrl += diffDeriv; // integrate the rate of change

  if (diffIntgrl >= 100 ) // limit integral value to prevent windup
  { 
  diffIntgrl = 0;
  }
  if (diffIntgrl <= -100 )
  { 
  diffIntgrl = 0;
  }


  speedRight = BASESPEED + (lineDiff * PGAIN) + (diffDeriv * DGAIN) + (diffIntgrl * IGAIN);
  if (speedRight < 0)
    speedRight = 0;
  if (speedRight > 100)
    speedRight = 100;
  speedLeft = BASESPEED - (lineDiff * PGAIN) - (diffDeriv * DGAIN) - (diffIntgrl * IGAIN);
  if (speedLeft < 0)
    speedLeft = 0;
  if (speedLeft > 100)
    speedLeft = 100;

  sparki.motorRotate(MOTOR_LEFT, DIR_CCW, speedLeft);
  sparki.motorRotate(MOTOR_RIGHT, DIR_CW, speedRight);

  sparki.clearLCD(); // wipe the screen
  
  sparki.print("Line Left: "); // show left line sensor on screen
  sparki.println(lineLeft);
  
  sparki.print("Line Center: "); // show center line sensor on screen
  sparki.println(lineCenter);
  
  sparki.print("Line Right: "); // show right line sensor on screen
  sparki.println(lineRight);

  sparki.print("lineDiff: ");
  sparki.println(lineDiff);

  sparki.print("final_lineDiff: ");
  sparki.println(final_lineDiff);

  sparki.print("diffDeriv: ");
  sparki.println(diffDeriv);

  sparki.print("diffIntgrl: ");
  sparki.println(diffIntgrl);



  sparki.updateLCD(); // display all of the information written to the screen

}
