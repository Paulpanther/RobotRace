/*
 * Hapkit control skeleton code
 */
#include <Arduino.h>
#include <HardwareSerial.h>

//Motor Pins
int pwmPin = 6; // PWM output pin for motor 1
int dirPin1 = 7; // direction output pin for motor 1
int dirPin2 = 8; // direction output pin for motor 1

//Encoder Pins
int encoderPin = A2;
int ledPin = 3;

//PID
int targetPos = 0;
double Kp = 0.4; //try between 0 and 1
double Ki = 0.00; //try between 0.00 and 0.10
double Kd = 6; //try between 0 and 10

//Kd parameters
int prevError = 0, vError;

//Ki parameters
int errorAccumulatorMax = (int)(255/Ki);
int errorAccumulator = 0;
unsigned long updatePeriod_I = 62L*100L, updatePeriod_D = 62L*1L, serialPrintPeriod = 62L*100L;; // clockrate scaling * ms
unsigned long currentTime, lastUpdateTime_I = 0, lastUpdateTime_D = 0, lastUpdateTime_Print = 0;
 
//Encoder Tracking
int lastRawPos = 0;
int lastLastRawPos = 0;
const int flipThresh = 700;  // threshold to determine whether or not a flip over the 180 degree mark occurred
boolean flipped = false;
int flipNumber = 0;     // keeps track of the number of flips over the 180deg mark
int tempOffset = 0;
int handlePos = 0;    //handle position


//--------------------------------------------
// Initial setup
//--------------------------------------------
void setup() {
  initializeMotor();

  //initialize encoder pins
  pinMode(ledPin, OUTPUT);
  pinMode(encoderPin, INPUT);
  Serial.begin(9600);
}


//--------------------------------------------
// Main Loop
//--------------------------------------------
void loop() { 
  
  //encoder
  //read in encoder position as handlePos
  int lastPos = handlePos;
  update_encoder_position_hapkit();
  //output encoder value to LED
  int ledValue = map(handlePos, -4000, 4000, 0, 255);
  analogWrite(ledPin, ledValue);


//=================================
//
// Edit control code starting here!
//
//=================================
  int motorOutput = 0;
  int error = 0;
  
  //Bang-bang here
//  if (handlePos > 0)
//     motorOutput = 0;
//  else
//     motorOutput = 255;

  //P-control here
  error = targetPos - handlePos;
  motorOutput += Kp * error;

  
  // I-control here
  currentTime = millis();
  if (currentTime - lastUpdateTime_I > updatePeriod_I) 
  {
      errorAccumulator = max(-errorAccumulatorMax, min(errorAccumulatorMax, errorAccumulator+error));
      lastUpdateTime_I = currentTime;
  }
  motorOutput +=  Ki * errorAccumulator;


  //D-control here
  currentTime = millis();
  if (currentTime - lastUpdateTime_D > updatePeriod_D) 
  {
      vError = error - prevError;
      prevError = error;
      lastUpdateTime_D = currentTime;
  }
  if(abs(vError) > 3) // simple filter for small noise
      motorOutput += Kd * vError;

//debug print
currentTime = millis();
  if (currentTime - lastUpdateTime_Print > serialPrintPeriod) // executes every ~100ms 
  {
      Serial.println(motorOutput); 
      lastUpdateTime_Print = currentTime;
  }
      
//=================================
//
// End main edit zone!
//
//=================================

  //motor 
  motorOutput = max(-255, min(255, motorOutput)); //cap motor output
  if(motorOutput > 0) //set motor direction
  {
    digitalWrite(dirPin1, LOW);  // set direction pin 1
    digitalWrite(dirPin2, HIGH);  // set direction pin 2
  } else {
    digitalWrite(dirPin1, HIGH);  // set direction pin 1
    digitalWrite(dirPin2, LOW);  // set direction pin 2
  }
  analogWrite(pwmPin, abs(motorOutput)); //write to motor
}

//===========================================
//
// HELPER FUNCTIONS
//
//===========================================


//--------------------------------------------
// Initialize Motor
//--------------------------------------------
void initializeMotor() {
  //set motor pin modes
  pinMode(pwmPin, OUTPUT);
  pinMode(dirPin1, OUTPUT);
  pinMode(dirPin2, OUTPUT);

  // set PWM frequency
  setPwmFrequency(pwmPin,1);

  // Initialize motor 
  analogWrite(pwmPin, 0);     // set to not spinning voltage
  digitalWrite(dirPin1, LOW);  // set direction
  digitalWrite(dirPin2, HIGH);  // set direction
}



//--------------------------------------------
// Pwm Frequency
// From https://playground.arduino.cc/Code/PwmFrequency
//--------------------------------------------
void setPwmFrequency(int pin, int divisor) {
  byte mode;
  if(pin == 5 || pin == 6 || pin == 9 || pin == 10) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 64: mode = 0x03; break;
      case 256: mode = 0x04; break;
      case 1024: mode = 0x05; break;
      default: return;
    }
    if(pin == 5 || pin == 6) {
      TCCR0B = TCCR0B & 0b11111000 | mode;
    } else {
      TCCR1B = TCCR1B & 0b11111000 | mode;
    }
  } else if(pin == 3 || pin == 11) {
    switch(divisor) {
      case 1: mode = 0x01; break;
      case 8: mode = 0x02; break;
      case 32: mode = 0x03; break;
      case 64: mode = 0x04; break;
      case 128: mode = 0x05; break;
      case 256: mode = 0x06; break;
      case 1024: mode = 0x7; break;
      default: return;
    }
    TCCR2B = TCCR2B & 0b11111000 | mode;
  }
}



//--------------------------------------------
// Read Encoder from Hapkit
//--------------------------------------------
void update_encoder_position_hapkit(){
    int rawPos = analogRead(encoderPin);  //current raw position from MR sensor
  
    // Calculate differences between subsequent MR sensor readings
    int rawDiff = rawPos - lastRawPos;          //difference btwn current raw position and last raw position
    int lastRawDiff = rawPos - lastLastRawPos;  //difference btwn current raw position and last last raw position
    int rawOffset = abs(rawDiff);
    int lastRawOffset = abs(lastRawDiff);
  
    // Update position record-keeping vairables
    lastLastRawPos = lastRawPos;
    lastRawPos = rawPos;
  
    // Keep track of flips over 180 degrees
    if((lastRawOffset > flipThresh) && (!flipped)) { // enter this anytime the last offset is greater than the flip threshold AND it has not just flipped
        if(lastRawDiff > 0) {        // check to see which direction the drive wheel was turning
            flipNumber--;              // cw rotation 
        } else {                     // if(rawDiff < 0)
            flipNumber++;              // ccw rotation
        }
        if(rawOffset > flipThresh) { // check to see if the data was good and the most current offset is above the threshold
            handlePos = rawPos + flipNumber*rawOffset; // update the pos value to account for flips over 180deg using the most current offset 
            tempOffset = rawOffset;
        } else {                     // in this case there was a blip in the data and we want to use lastactualOffset instead
            handlePos = rawPos + flipNumber*lastRawOffset;  // update the pos value to account for any flips over 180deg using the LAST offset
            tempOffset = lastRawOffset;
        }
        flipped = true;            // set boolean so that the next time through the loop won't trigger a flip
    } else {                        // anytime no flip has occurred
        handlePos = rawPos + flipNumber*tempOffset; // need to update pos based on what most recent offset is 
        flipped = false;
    }

}
