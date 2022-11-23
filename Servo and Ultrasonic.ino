//------------- Servo -----------------
#include <Servo.h>
Servo Rotate;
//--------------Ultrasonic-------------
int Ear = 7; //Echo or Echo Pin
int Shout = 8;   //Trig or Trigger pin
float distance;
float length;
float read = 0;
float newread = 0;
int direction;
//motor one
int enA = 11; //int1 & int2
int Go1 = 10; //int1
int Back1 = 9;  //int2
// motor two
int enB = 3;  //int3 & int4
int Go2 = 5;  //int4
int Back2 = 6;  //int3

//===============================================================================================//
void setup() {
  pinMode(Shout, OUTPUT);
  pinMode(Ear, INPUT);
  Rotate.attach(2);
  //Set all Motors 
  pinMode(enA, OUTPUT); //POWER
  pinMode(enB, OUTPUT); //POWER
  pinMode(Go1, OUTPUT); //DIRECTION A MOTOR A
  pinMode(Back1, OUTPUT); //DIRECTION B MOTOR A
  pinMode(Back2, OUTPUT); //DIRECTION C MOTOR B 
  pinMode(Go2, OUTPUT); //DIRECTION C MOTOR B
}

//------MOTOR DIRECTION CONTROLLER------//
int MotorHighLow(int ENABLE, int DISABLE, int ENABLE2, int DISABLE2){
  digitalWrite( ENABLE,  HIGH);
  digitalWrite( DISABLE,  LOW);
  digitalWrite( ENABLE2, HIGH);
  digitalWrite( DISABLE2, LOW);
}
//------MOTOR ENGINE ON AND OFF--------//
bool Motor(bool signal, int speed){ //Engine ON
  if (signal==true){
    analogWrite(enA, speed);
    analogWrite(enB, speed);
  }else if(!signal){                //Engine OFF
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    digitalWrite(Go1, LOW);
    digitalWrite(Back1, LOW);
    digitalWrite(Back2, LOW);
    digitalWrite(Go2, LOW);
    delay(1000);
  };
}
//------ULTRASONIC READING---------
int Ultrasonic(){
  digitalWrite(Shout, LOW);
  delayMicroseconds(2);
  //CLEAR Trigger Pin
  digitalWrite(Shout, HIGH);
  delayMicroseconds(10);
  digitalWrite(Shout, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  return (distance = (pulseIn(Ear, HIGH)) * 0.034/2); //since the sounds travel 2x(forward and backward) after bouncing, the distance/duration is technically read as double. need to dvide by 2.
};

//---------SERVO AND ULTRASONIC SYNCRHONIZATION----------//
int looping(){
  Rotate.write(0);
  delay(1000);
  for(int i=0; i<= 180; i++){
    Rotate.write(i);
    newread = Ultrasonic();
    if (read < newread){
      read = newread;
      direction = i;
    }
  delay(5);
  }
  delay(500);
  Rotate.write(90); //rotate servo back to 90 degree
  return direction;
}

//---------------------LOOP------------------------------------//
void loop() {
  length = Ultrasonic();
  if (length < 15){
    Motor(false, 0); //STOP THE ENGINE
    //Go Back
    MotorHighLow(Back2,Go2, Back1, Go1);
    Motor(true, 40);
    delay(1200);
    Motor(false,0);
    //GET BEST DIRECTION
    direction = looping(); //Rotate SERVO TO GET DATA IN SURROUNDING  
    //FIX MOBOT DIRECTION
    delay(1000);
    if (direction < 90){ //0-89 - RIGHT
        MotorHighLow(Back1, Go1, Go2, Back2 );  //FIX MOBOT WHEEL FOR ROTATION     
    }else{               //90-180 - LEFT
        MotorHighLow(Back2, Go2, Go1, Back1); //FIX MOBOT WHEEL FOR ROTATION       
    }
    delay(500);
    Motor(true, 60); //ROTATE THE MOBOT
    for (int i=0; i<= 250;i=i+1){
      delay(2);
    }
    Motor(false, 0);
  }
  
  //FORWARD
  MotorHighLow(Go1, Back1, Go2, Back2); //MOTOR A FORWARD
  Motor(true, 55);        //Engine ON
  delay(200);
}
