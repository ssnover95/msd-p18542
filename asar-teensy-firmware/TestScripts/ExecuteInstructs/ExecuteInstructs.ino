/*
 * ExecuteInstructs.ino
 * Author: Aaron Mallabar
 * Date: March 9, 2018
 * Description: This Script takes in a set of instructions from the xbee.
 *              And executes the instructions using the the encoder. 
 *              (Set of five instructs)
 */
#include "Arduino.h"
#include "Motor.h"
#include "XBEE.h"
#include "PITimer.h"

using namespace ASAR;

const int LED = 13;
const double Period = .01;
bool Flag = false;
bool instructDone = false;
int CurrentInstruct = 1;
double displacement = 0;
double angle = 0;
const int TURN_SPEED = 150;

void callback();
void executeCurrentInstruct();
void printStatus();

ASAR::XBEE myXBEE;
ASAR::MOTOR myMOTOR;

void setup() 
{
  Serial1.begin(115200);
  Serial.begin(115200);
  delay(1000);
  pinMode(LED, OUTPUT);
  PITimer1.period(Period);
  PITimer1.start(callback);
  myMOTOR.initEncoder();
  Serial1.println("Waiting...");
  while(myXBEE.action[1] == 0)
  {
    myXBEE.getInstructions();
  }
  Serial1.println("My Actions Are..");
  for(int i = 1; i < myXBEE.instructTotal; i++)
  {
    Serial1.print(i); Serial1.print(". ");
    Serial1.println(myXBEE.action[i], HEX);
  }
  
}

void loop()
{
  while(Flag)
  {
    Flag = false;
    instructDone = false;
    displacement = myMOTOR.getPosition(); //Initialize the displacement
    angle = myMOTOR.getAngle();   //Initializes the angle
    executeCurrentInstruct();
    if(instructDone)
    {
      printStatus();
      CurrentInstruct++;
      myMOTOR.initEncoder();
      digitalWrite(LED, !digitalRead(LED));
      myMOTOR.Stop();
      delay(250);
    }
    if (Serial1.available())
    {
      CurrentInstruct = 1;
      myXBEE.initInstruction();
      myXBEE.getInstructions();
      Serial1.println("My Actions Are..");
      for(int i = 1; i < myXBEE.instructTotal; i++)
      {
        Serial1.print(i); Serial1.print(". ");
        Serial1.println(myXBEE.action[i], HEX);
      }
    }
  }
}

/*Used to start the main loop through timer interrupt*/
void callback()
{
  Flag = true;
}

/*Sends the current instruction to the motors*/
void executeCurrentInstruct()
{
  switch (static_cast<XBEE::INSTRUCT_BYTE>(myXBEE.action[CurrentInstruct]))
  {
    case XBEE::INSTRUCT_BYTE::ACTION_LEFT: //Turn Left
      myMOTOR.LeftTurn(TURN_SPEED);
      if (angle <= -myXBEE.angle[CurrentInstruct])
      {
        instructDone = true;
      }
      break;
    case XBEE::INSTRUCT_BYTE::ACTION_RIGHT: //Turn Right
      myMOTOR.RightTurn(TURN_SPEED);
      if (angle >= myXBEE.angle[CurrentInstruct])
      {
        instructDone = true;
      }
      break;            
    case XBEE::INSTRUCT_BYTE::ACTION_FORWARD: //Go Forward
      myMOTOR.Forward(myXBEE.speedy[CurrentInstruct]);
      if (displacement >= (myXBEE.distance[CurrentInstruct]))
      {
        instructDone = true;
      }
      break;  
    case XBEE::INSTRUCT_BYTE::DONE_EXECUTION: //if No more instructions
      myMOTOR.Stop();
      //Serial1.println("All Done"); //Prints Continuously     
    default: //If the action type is not recognized
      myMOTOR.Stop();
      break;  
  }
}


void printStatus()
{
  Serial1.print("Instruction: #"); Serial1.println(CurrentInstruct);
  Serial1.print("Displacement: "); Serial1.print(displacement); Serial1.println(" cm");
  Serial1.print("Angle: "); Serial1.print(angle); Serial1.println (" deg");
  //Serial.print("Error: "); Serial.print(straightError); Serial.println(" Degrees/Meter");
  Serial1.println(myXBEE.action[CurrentInstruct], HEX);
}

