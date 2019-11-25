/*** Header Blocks ***
 *FileName: Capstone_Design_Project
 *Title: Automatic Door
 *Description: Using the PIR Motion Sensor and other various components to detect people to walk through door automatically.
 *Hardware Required: Arduino UNO R3
 *Arduino 1.8.9
 *Programmer Name: Huilin Chen
 *Date: 10/3/2019
 *Code Version: 0.0.0.10
 ***/

//libraries 
#include <Servo.h>
Servo myservo;

//Define pins
/*Potentiometer Pins*/
int potPin = A0; //This is an Analog pin used to connect the potentiometer
/*PIR Motion Sensor*/
int inputPir = 2; //The input pin for PIR sensor
/*Slide Switch*/
int switchPin = 12; //switch is connected to pin 12
/*LED*/
int ledPin = 11; //Pin for LED
/*States & status*/ 
int pirState = LOW; //no Motion Detected
int val = 0; //Reading the pin status for PIR Sensor
int Servoval = 0; //Reading the pin status for Servo Motor
int switchState = 0; //Reading in the switch status
char incoming_Value = 0; //Storing the incoming value

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Data communication
  myservo.attach(9); //Servo motor is attached to pin 9
  pinMode(ledPin, OUTPUT); // LED Pin output
  pinMode(inputPir, INPUT); //declaring sensor as input
  pinMode(switchPin, INPUT); //declaring switch as input
}

void loop() {
  // put your main code here, to run repeatedly:
  val = digitalRead(inputPir); //Reading in the input value for PIR
  Servoval = analogRead(potPin); //Reading in the value for potentiometer between 0 and 1023
  switchState = digitalRead(switchPin); //Reading in the state of the switch value

  switching_System();//To turn on and off the system using a switch
  
  if(Serial.available() > 0){
    incoming_Value = Serial.read();// Reading the incoming dats and storing it into variable incoming_Value
    Serial.print(incoming_Value);
    Serial.print("\n");
    if(incoming_Value == '1'){
      Serial.print("LED is on");
      digitalWrite(13, HIGH);
    }
    else if(incoming_Value == '2'){
      Serial.print("LED is off");
      digitalWrite(13,LOW);
    }
    else if(incoming_Value == '3'){
      Serial.print("System is now on");
      motion_Servo();
    }
    else if(incoming_Value == '4'){
      Serial.print("System is now off");
      exit(0);
    }
  }
}

void switching_System(){
  if(switchState == HIGH){
    Serial.println("System is activated");
    motion_Servo();
  }
  else
  {
    Serial.println("System is deactivated");
    motion_Servo(0);
  }
}

void motion_Servo(){
  //Data processing
  if (val == HIGH){ //checking if the input for PIR is HIGH
    digitalWrite(ledPin, HIGH); //If Input PIR is HIGH, LED is On
    if (pirState == LOW){
      Serial.println("Motion detected!");
      pirState = HIGH;
      Servoval = map(Servoval,0,1023,0,180); //Scaling it to use it with the Servo motor from 180 to 0)
      myservo.write(Servoval);
    }
    else{
      digitalWrite(ledPin, LOW); // Turning LED OFF
      if(pirState == HIGH){
        delay(15);
        Serial.println("Motion has ended");
        pirState = LOW;
        Servoval = map(Servoval,0,1023,180,0);
        myservo.write(Servoval);
      }
    }
  }  
}
