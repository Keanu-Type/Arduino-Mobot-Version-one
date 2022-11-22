//------------- Servo -----------------
#include <Servo.h>
Servo Rotate;
int degree;
//--------------Ultrasonic-------------
int Ear = 7; //Echo or Echo Pin
int Shout = 8;   //Trig or Trigger pin
float distance;
float length;
float read = 0;
float newread = 0;
int direction;
//-------------Motor------------------
//motor one
int enA = 11;
int in1 = 10;
int in2 = 9;
// motor two
int enB = 3;
int in3 = 6;
int in4 = 5;
bool signalON = true;
bool signalOFF = false;

//===============================================================================================//
void setup() {
  // put your setup code here, to run once:
  pinMode(Shout, OUTPUT);
  pinMode(Ear, INPUT);
  Rotate.attach(2);
  Serial.begin(9600);

  //Set all Motors 
  pinMode(enA, OUTPUT); //POWER
  pinMode(enB, OUTPUT); //POWER
  pinMode(in1, OUTPUT); //DIRECTION A MOTOR A
  pinMode(in2, OUTPUT); //DIRECTION B MOTOR A
  pinMode(in3, OUTPUT); //DIRECTION C MOTOR B 
  pinMode(in4, OUTPUT); //DIRECTION C MOTOR B
  
}

//------MOTOR DIRECTION CONTROLLER------//
int MotorHighLow(int ENABLE, int DISABLE, int ENABLE2, int DISABLE2){
  digitalWrite( ENABLE,  HIGH);
  digitalWrite( DISABLE,  LOW);
  digitalWrite( ENABLE2, HIGH);
  digitalWrite( DISABLE2, LOW);
}
//------MOTOR ENGINE ON AND OFF--------//
bool Motor(bool signal){ //Engine ON
  if (signal==true){
    analogWrite(enA, 70);
    analogWrite(enB, 70);
  }else if(!signal){ //Engine OFF
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
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
  delay(10);
  }   
  delay(1000);
  return direction;
}

//---------------------LOOP------------------------------------//
void loop() {
  length = Ultrasonic();
  if (length < 15){
    Motor(signalOFF); //STOP THE ENGINE
    direction = looping(); //Rotate SERVO TO GET DATA IN SURROUNDING  
    Rotate.write(90); //ROTATING SERVO BACK TO CENTER/90 degree
    //FIX MOBOT DIRECTION
    delay(1000);
    if (direction < 90){ //0-89 - RIGHT
        MotorHighLow(in3, in4, in1, in2); //FIX MOBOT WHEEL FOR ROTATION
        direction = 90 - direction;        
    }else{               //90-180 - LEFT 
        MotorHighLow(in2, in1, in4, in3 );  //FIX MOBOT WHEEL FOR ROTATION
        direction = 180 - direction;
    }
    for (float i=0.00; i<(direction);i=i+0.1){
      Motor(signalON); //ROTATE THE MOBOT
      delay(2);
    }
    Motor(signalOFF);
  }
  //FORWARD
  MotorHighLow(in1, in2, in4, in3); //MOTOR A FORWARD
  Motor(signalON);        //Engine ON
  delay(300);
}
