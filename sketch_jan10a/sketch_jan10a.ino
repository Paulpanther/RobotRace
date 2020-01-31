#include <Arduino.h>
#include <HardwareSerial.h>

//Motor Pins
int dirLeft1 = 5; // direction output pin for motor 1
int dirLeft2 = 6; // direction output pin for motor 1

int dirRight1 = 10;
int dirRight2 = 9;

int sensor2_pin = A2;
int sensor1_pin = A1;

/* Beispielwerte
 * Schwarz:   370-380
 * Weiß:      97-161 ca. 120 im Durchschnitt
 * Grau:      187-266
 * 
 * min: 0
 * max: 900
 */
const int min_value = 0;
const int max_value = 900;
 
const int points_length = 21;
int last_points[points_length];

const int bucket_length = 300;
const int bucket_size = 3;
int bucket_points[bucket_length] = { 0 };

int black_value = 0;
int white_value = 0;
int grey_value = 0;


int sort_desc(const void *cmp1, const void *cmp2){
  int a = *((int *)cmp1);
  int b = *((int *)cmp2);
  return b - a;
}

int median_lastPoints(){
  qsort(last_points, points_length, sizeof(last_points[0]), sort_desc);
  return last_points[points_length/2];
}

void find_max(){
    int third, second, first; 
    for (int i = 0; i < bucket_length ; i ++){
        if (bucket_points[i] > bucket_points[first]){ 
            third = second; 
            second = first; 
            first = bucket_points[i]; 
        }else if (bucket_points[i] > bucket_points[second]){ 
            third = second; 
            second = bucket_points[i]; 
        }else if (bucket_points[i] > bucket_points[third]) 
            third = bucket_points[i]; 
    }
    black_value = bucket_points[first];
    grey_value = bucket_points[second];
    white_value = bucket_points[third];
}

void initialize_buckets(){
  int point = 0;
  for(int i = 0; i < bucket_length; i++){
    point = analogRead(sensor2_pin);
    
  }
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
  
  Serial.print("median ");
  Serial.println(median_lastPoints());
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
