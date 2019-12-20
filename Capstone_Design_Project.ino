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
#include "SoftwareSerial.h"
SoftwareSerial myBluetooth(3,4); //RX = pin 3, Tx = pin 4
Servo myservo;

//Define pins
/*Potentiometer Pins*/
int potPin = A5; //This is an Analog pin used to connect the potentiometer
/*PIR Motion Sensor*/
int inputPir = 2; //The input pin for PIR sensor
/*Slide Switch*/
int switchPin = 12; //switch is connected to pin 12
/*LED*/
int ledRedPin = 11; //Pin for Red LED
int ledGreenPin = 13; //Pin for Green LED
/*States & status*/ 
int pirState = LOW; //initallizing the PIR sensor to be set to LOW
int pirVal = 0; //Reading the pin status for PIR Sensor
int servoVal = 0; //Reading the pin status for Servo Motor
int switchState = 0; //Reading in the switch status
int potVal = 0; //reading in the analog for potentiometer
char incoming_Values; //Storing the incoming value from the bluetooth
/*potentiometer's output value for speed*/
int outputValue = 0; 
/*Servo Motor Position Variable*/
int pos = 0;

/*Function be set on the bottom of loop*/
void bluetooth_App();
void motion_Servo();
void switching_System();

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //Data communication
  myservo.attach(9); //Servo motor is attached to pin 9
  pinMode(ledRedPin, OUTPUT); // LED Pin output
  pinMode(ledGreenPin,OUTPUT);
  pinMode(inputPir, INPUT); //declaring sensor as input
  pinMode(switchPin, INPUT); //declaring switch as input
  myBluetooth.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  pirVal = digitalRead(inputPir); //Reading in the input value for PIR
  switchState = digitalRead(switchPin); //Reading in the state of the switch value
  potVal = analogRead(potPin); // analog read the potentiometer's value
  outputValue = map(potVal, 0, 1023, 0, 20); //mapping the potentiometer to a certain limit of number
  
  switching_System();//To turn on and off the system using a switch
  bluetooth_App();//Using bluetooth to control the LED and Door
}

//Bluetooth to the APP
void bluetooth_App(){
  //bluetooth to App
  if(myBluetooth.available()>0){ //any value that is greater than 0
    incoming_Values = myBluetooth.read(); //we are now storing the value
    if(incoming_Values == 'a'){ //when the app sends 'a' 
      digitalWrite(ledRedPin, HIGH); //LED would be on
    }
    if(incoming_Values == 'b'){ //When the app sends 'b'
      digitalWrite(ledRedPin, LOW); //LED would be off
    }
    if(incoming_Values == 'c'){ //When the app sends 'c'
      motion_Servo(); //motion sensor is on
    }
    if(incoming_Values == 'd'){ //when the app sends 'd'
    }
  }
}

//turning the switch on and off
void switching_System(){
  if(switchState == HIGH){ //When the switch is HIGH
    myservo.write(0); //starting position will always be 0
    digitalWrite(ledGreenPin,HIGH); //Green LED is on 
    motion_Servo(); //Starts the motion sensor on
    
  }
  else
  {
    myservo.write(0); //Otherwise, servo starts at position 0
    digitalWrite(ledGreenPin,LOW); //Green LED is OFF
  }
}

//servo motor is moving by the PIR Sensor
void motion_Servo(){
  //Data processing
  if (pirVal == HIGH){ //checking if the input for PIR is HIGH
    digitalWrite(ledRedPin, HIGH); //If Input PIR is HIGH, LED is On
    if (pirState == LOW){
      Serial.println("Motion detected!");
      pirState = HIGH;
      for (pos = 0; pos <= 90; pos++) {  //Goes from 0 to 90 degrees
        myservo.write(pos);              // tell servo to go to position in variable 'pos'
        delay(outputValue);              // uses the potentiometer to change speed in ms
      }
    }
  }
  
  else{
    digitalWrite(ledRedPin, LOW); // Turning LED OFF
      if(pirState == HIGH){
        pirState = LOW;
        Serial.println("Motion has ended");
        for (pos = 90; pos >= 0; pos--) {  // goes from 90 degrees to 0 degrees
          myservo.write(pos);              // tell servo to go to position in variable 'pos'
          delay(outputValue);              // uses the potentiometer to change speed in ms
        }
      }
}} 
