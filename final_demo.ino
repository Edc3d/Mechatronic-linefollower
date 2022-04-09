/*
 * Wiring scheme
 *  Sensor slot / arduino input
 *  slot 2      / A0
 *  slot 4      / A1
 *  slot 6      / A2
 *  slot 8      / A3 
 *  slot 10     / A4 
 *  slot 11     / A7
 *  slot 12     / A8
 *  slot 13     / A9
 *  slot 14     / A10
 */

// ---- lib -------
#include <QTRSensors.h> 
#include <Wire.h> 
#include <Adafruit_MotorShield.h>

// ---- init motors ----------------------
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
Adafruit_DCMotor *left_motor = AFMS.getMotor(1);
Adafruit_DCMotor *right_motor = AFMS.getMotor(4);
QTRSensors qtr;

//----- PID PARAM ------ PLAY AROUND PARAMS

float Kp = 0.20; 
float Kd = 0;
float Ki = 0;
int MAX_PWR = 200; // 255 is maximum voltage to motors. [-255,255]
int Set_point = 7589; //change this for middle line

//---------- init varibles -------------------
int pos = 0;
int last_error = 0;
int sen_2 = 0;
int sen_4 = 0;
int sen_6 = 0;
int sen_8 = 0;
int sen_10 = 0;
int sen_11 = 0;
int sen_12 = 0;
int sen_13 = 0;
int sen_14 = 0;

void setup() 
 {
  Serial.begin(9600);
  AFMS.begin();
  delay(5000);
 }
  
void loop()
{
     
      readdata(); // sensor read function.

      //------- PID ALGO -------------------
      int error =  pos - Set_point; //
      int sum_error = sum_error + error;
      int adju = Kp*error + Ki*sum_error + Kd*(error - last_error);
      last_error = error;
      //----------------------------------
      
       //-----------------Motor------------------
       // sensor aray tilted to the left --> turn robot other way by slowing down one wheel increasing speed on the other.
       // Same thing for tilt to the right.
       // speed can noly be 0 or MAX_PWR varible.
       left_motor -> setSpeed(constrain(MAX_PWR - adju, 0 , MAX_PWR));
       left_motor -> run(FORWARD);
       right_motor -> setSpeed(constrain(MAX_PWR + adju, 0 , MAX_PWR));
       right_motor -> run(FORWARD);

    

}

void readdata() 
{
sen_2 = analogRead(A0); 
sen_4 = analogRead(A1); 
sen_6 = analogRead(A2); 
sen_8 = analogRead(A3); 
sen_10 = analogRead(A7); 
sen_11 = analogRead(A8); 
sen_12 = analogRead(A9);
sen_13 = analogRead(A10); 
sen_14 = analogRead(A11); 

//---- READ FUNC -----------  
//build on pololu readLine func https://www.pololu.com/docs/0J19/all.
// returns estimated position of the line value of 2000 indicates the line is under sen_2 .... value 14000 indicates the line is under sen_14
pos = ((2000*sen_2 + 4000*sen_4 + 6000*sen_6 + 7000*sen_8 + 8000*sen_10 + 9000*sen_11 + 10000*sen_12 + 12000*sen_13 + 14000*sen_14) / (sen_2+sen_4+sen_6+sen_8+sen_10+sen_11+sen_12+sen_13+sen_14));

}
