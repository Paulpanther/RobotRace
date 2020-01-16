
int sensor2_pin = A2;
int sensor1_pin = A1;

int encoder1_pin = A4;
int encoder2_pin = A5;

int motor_left_pin = 7;
int motor_right_pin = 8;

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

void setup() {
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
