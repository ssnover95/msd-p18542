/*
 * ExecuteInstructs.ino
 * Author: Aaron Mallabar
 * Date: March 9, 2018
 * Description: This Script takes in a set of instructions from the xbee.
 *              And executes the instructions using the the encoder. 
 *              (Set of five instructs)
 */
#include "Arduino.h"
#include "MOTOR.h"
#include "XBEE.h"
#include "PITimer.h"

const int LED = 13;
const double Period = .5;
bool Flag = false;
bool instructDone = false;
int CurrentInstruct = 1;
double disp = 0;
double angle = 0;


void callback();

ASAR::XBEE myXBEE;
ASAR::MOTOR myMOTOR;

void setup() 
{
  Serial.begin(9600);
  delay(1000);
  pinMode(LED, OUTPUT);
  PITimer1.period(Period);
  PITimer1.start(callback);
  myMOTOR.initEncoder();
  Serial.println("Waiting...");
  while(myXBEE.action[1] == 0)
  {
    myXBEE.getInstructions();
  }
  Serial.println("My Actions Are..");
  for(int i = 1; i < myXBEE.instructTotal; i++)
  {
    Serial.print(i); Serial.print(". ");
    Serial.println(myXBEE.action[i], HEX);
  }
  
}

void loop()
{
  while(Flag)
  {
    Flag = false;
    instructDone = false;
    //myXBEE.GetInstructions(5);
   // Serial.print("Instruction One: "); Serial.println(myXBEE.action[1], HEX);
    disp = myMOTOR.getPosition();
    angle = myMOTOR.getAngle();
    Serial.print("Instruction: #"); Serial.println(CurrentInstruct);
    Serial.print("Displacement: "); Serial.print(disp); Serial.println(" cm");
    Serial.print("Angle: "); Serial.print(angle); Serial.println (" deg");
    Serial.println(myXBEE.action[CurrentInstruct], HEX);
    switch (myXBEE.action[CurrentInstruct])
    {
      case 0xAA : //Turn Left
        myMOTOR.LeftTurn(3);
        if (abs(angle) >= myXBEE.angle[CurrentInstruct])
        {
          Serial.print("Angle: "); Serial.println(angle);
          Serial.print("Goal Angle: "); Serial.println(myXBEE.angle[CurrentInstruct]);
          instructDone = true;
        }
        break;
      case 0xBB : //Turn Right
        myMOTOR.RightTurn(150);
        if (abs(angle) >= myXBEE.angle[CurrentInstruct])
        {
          Serial.print("Angle: "); Serial.println(angle);
          Serial.print("Goal Angle: "); Serial.println(myXBEE.angle[CurrentInstruct]);
          instructDone = true;
        }
        break;            
      case 0xCC : //Go Forward
        myMOTOR.Forward(myXBEE.speedy[CurrentInstruct]);
        if (disp >= (myXBEE.distance[CurrentInstruct])/10)
        {
          instructDone = true;
        }
        break;           
      default: //If the action type is not recognized
        myMOTOR.Stop();
        break;  
    }
    if(instructDone)
    {
      CurrentInstruct++;
      myMOTOR.initEncoder();
      digitalWrite(LED, !digitalRead(LED));
    }
  }
}

void callback()
{
  //digitalWrite(LED, !digitalRead(LED));
  Flag = true;
}
