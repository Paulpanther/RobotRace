#include <Arduino.h>
#include <HardwareSerial.h>

//Motor Pins
int dirLeft1 = 5; // direction output pin for motor 1
int dirLeft2 = 6; // direction output pin for motor 1

int dirRight1 = 10;
int dirRight2 = 9;

int sensor2_pin = A2;
int sensor1_pin = A1;

const int max_value = 900;
const int array_length = 120;

typedef struct _sensor {
  int black_value;
  int grey_value;
  int white_value;

  int black_grey_thresh;
  int white_grey_thresh;

  int arr[array_length];
} Sensor;

Sensor s1 = Sensor(0, 0, 0, 0, 0, {0});
Sensor s2 = Sensor(0, 0, 0, 0, 0, {0});
Sensor s3 = Sensor(0, 0, 0, 0, 0, {0});

void find_max(){
    int localMax[array_length/2] = {0};
    int pointerToCurrentLocalMax = 0;

    //save all local maxima in localMax[]
    for (int i = 1; i < array_length; i++){
        if (arr[i-1]< arr[i] && arr[i]>arr[i+1]){
           localMax[pointerToCurrentLocalMax] = i;
           pointerToCurrentLocalMax ++;
        }
    }

    int max1 = 0, max2 = 0, max3 = 0;
    for (int i = 0; i < array_length/2; i++){
        if (arr[localMax[i]] > arr[max1]){
            max3 = max2;
            max2 = max1;
            max1 = localMax[i];
        }else if (arr[localMax[i]] > arr[max2]){
            max3 = max2;
            max2 = localMax[i];
        }else if (arr[localMax[i]] > arr[max3]){
            max3 = localMax[i];
        }
    }

    black_value = std::min(max1, std::min(max2, max3));
    white_value = std::max(max1, std::max(max2, max3));
    grey_value = max1 + max2 + max3 - black_value - white_value;

    //compute thresholds
    black_grey_thresh = (int) ((black_value + grey_value)/ 2);
    white_grey_thresh = (int) ((white_value + grey_value)/ 2);
}

void addValueToBuckets(int value){
    arr[value*array_length/max_value]++;
}

int predict(int value) {
    
    if(num_prev_data == 1){
        for (int i = 0; i < array_length; ++i)
        {
            arr[i] = 0;
        }
    }
    addValueToBuckets(data[num_prev_data-1]);
    find_max();
    //output: 0 for black, 1 for gray and 2 for white.
    if(value*array_length/max_value < black_grey_thresh)return 0;
    if(value*array_length/max_value < white_grey_thresh)return 1;
    else return 2;
}

void drive(int motor_right, int motor_left){ //values from -255 to 255
  if(motor_right < 0){
    analogWrite(dirRight1, abs(motor_right));
    analogWrite(dirRight2, 0);
  }else{
    analogWrite(dirRight2, abs(motor_right));
    analogWrite(dirRight1, 0);
  }
  if(motor_left < 0){
    analogWrite(dirLeft1, abs(motor_left));
    analogWrite(dirLeft2, 0);
  }else{
    analogWrite(dirLeft2, abs(motor_left));
    analogWrite(dirLeft1, 0);
  }
}

void pwm_drive(int right, int left, int r1, int r2) {
  drive(right, left);
  delay(r1);
  drive(0, 0);
  delay(r2);
}

void forward() {
  pwm_drive(230, 254, 5, 3);
}

void turn_left() {
  pwm_drive(-170, 170, 5, 3);
}

void turn_right() {
  pwm_drive(-170, 170, 5, 3);
}

void setup() {
  initializeMotors();
  // put your setup code here, to run once:
  pinMode(sensor2_pin, INPUT);
  Serial.begin(9600);

  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  for(int i = 0; i < points_length; i++){
    delay(100);
    last_points[i] = analogRead(sensor2_pin);
  }
  
}

//--------------------------------------------
// Initialize Motor
//--------------------------------------------
void initializeMotors() {
  //set motor pin modes
  pinMode(dirRight1, OUTPUT);
  pinMode(dirRight2, OUTPUT);

  pinMode(dirLeft1, OUTPUT);
  pinMode(dirLeft2, OUTPUT);

  // set PWM frequency
  setPwmFrequency(dirRight1,1);
  setPwmFrequency(dirLeft1,1);
  setPwmFrequency(dirRight2,1);
  setPwmFrequency(dirLeft2,1);
}


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
