//------------- Servo -----------------
#include <Servo.h>
Servo Rotate;
int degree;
int taken;
//s
//--------------Ultrasonic-------------
int Ear = 7; //Echo or Echo Pin
int Shout = 8;   //Trig or Trigger pin
long duration; //float vs long = float write 3.0320143113431313112341 while long write 3.0201eUF 
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
//s
int MotorHighLow(int ENABLE, int DISABLE){
  digitalWrite( ENABLE, HIGH);
  digitalWrite( DISABLE, LOW);
}
//
bool Motor(bool signal){
  if (signal==true){
    //set engine speed
    analogWrite(enA, 75);
    analogWrite(enB, 75);
  }else if(!signal){
    //turn off engine
    analogWrite(enA, 0);
    analogWrite(enB, 0);
    //turnoff all signals
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
    delay(1000);
  };
}


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

int looping(){
  Rotate.write(0);
  delay(1000);
  for(int i=0; i<= 180; i++){
    Rotate.write(i);
    newread = Ultrasonic();
    Serial.print("Reading -------- ");
    Serial.println(newread);
    Serial.print("Directions: ");
    Serial.println(i);
    if (read < newread){
      read = newread;
      direction = i;
    }
  delay(2);
  }   
    Serial.println("");
  Serial.print(direction);
  Serial.print(" Servo mark // ");
  Serial.print(read);
  Serial.println(" cm");
  delay(1000);
  return read;
}

void loop() {
  length = Ultrasonic();

  Serial.print(length);
  Serial.println( " cm");
  if (length < 7){
    Motor(signalOFF); //STOP THE ENGINE
    delay(1000);
    Serial.println("SENSOR READING");
    //ROTATE SERVO
    length = looping();
    Serial.println("Rotating Back the Servo to 90");    
    Rotate.write(90);
    //FIX MOBOT DIRECTION
    if (direction < 90){ //0-89 - RIGHT
      for (int i=0; i < direction; i++ ){
        MotorHighLow(in3, in4);
        MotorHighLow(in1, in2);
        Motor(signalON);
        Serial.print("-");
        delay(2);
      }
    }else{               //90-180 - LEFT
      for (int i=0; i < (direction-90); i++ ){
        MotorHighLow(in2, in1);
        MotorHighLow(in4, in3);
        Motor(signalON);
        Serial.print("/");
        delay(2);
      }   
    }
    Motor(signalOFF);
  }
  Serial.print("");
  Serial.println("Moving");
  //FORWARD
  MotorHighLow(in1, in2); //MOTOR A FORWARD
  MotorHighLow(in4, in3); //MOTOR B REVERSE
  Motor(signalON);        //Engine ON
  delay(300);
 
}
