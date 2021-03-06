#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int flexSensorPin = A0; //analog pin 0
int temperaturePin = 1; //analog pin 1

const int keep = 5;
const float wash_time = 2700000;
const int threshold = 200;
int values[keep];
int averaged_values[keep];
int valcounter = 0;
float cleaning_start_time;
int previous_running_status = 0;
const int sample_rate_per_second = 4;
const int high_threshold = 500;
const int low_threshold = 200;
const int frequency_milliseconds = int(1000 / sample_rate_per_second);
int previous_value;

void setup(){
  setup_lcd();
  Serial.begin(9600);
  for(int i = 0; i < keep; ++i)
    values[i] = 0;
}

void loop(){
//  is_running();
  int value;
  int flexSensorvalue = analogRead(flexSensorPin); 
  //Serial.println(flexSensorvalue);
  values[++valcounter % keep] = flexSensorvalue;

  if( flexSensorvalue > high_threshold){
   value = 1;
   
   Serial.println("value");
   Serial.println(value);
   Serial.println("previous_value");
   Serial.println(previous_value);
   if(previous_value == -1){
     Serial.println("transition");
     register_transition();
   }
  } else if ( flexSensorvalue < low_threshold ){
   value = -1;
   if(previous_value == 1){
     Serial.println("transition");
     register_transition();
   }
  } else {
   value = 0; 
  }
  
//  Serial.print("range: ");
//  Serial.print(maxval-minval);
//  Serial.print(" max: ");
//  Serial.print(maxval);
//  Serial.print(" min: ");
//  Serial.println(minval);
    
  int running = 0;
//  if(maxval - minval > 200) {
//    running = 1;
//  }

  lcd.setCursor(0, 0);
  if(running == 1) {
    lcd.print("Cleaning Now!");
    if(previous_running_status == 0){
      reset_timer();
    }
  } else {
    lcd.print("Not Cleaning!");
  }
  

//  Serial.print("Sensor value: ");
//  Serial.print(flexSensorvalue);
//  Serial.print(" running: ");
//  Serial.println(running);

  loop_lcd();
  loop_temperature();
  previous_running_status = running;
  previous_value = value;
  
  is_complete();
  if(is_complete()){
    notify_complete();
  }
  delay(frequency_milliseconds); //just here to slow down the output for easier value
}

void register_transition(){
}
boolean is_running(){
  int required_transition_count;
  int rolling_average_count = 5;
  int values[rolling_average_count];
  for(int i = 0; i < rolling_average_count; ++i)
    values[i] = 0;
  int flexSensorvalue = analogRead(flexSensorPin); 
  //Serial.println(flexSensorvalue);
  values[++valcounter % keep] = flexSensorvalue;
  
}

void loop_temperature(){
  float voltage, degreesC;
  int degreesF;
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;

  lcd.setCursor(15, 1);
  lcd.print('F');
  int lcd_position = 15;
  while(degreesF)
  {
    lcd.setCursor(lcd_position - 1, 1);
    lcd.print(degreesF % 10);
    degreesF /= 10;
  }
}

float getVoltage(int pin)
{ 
  float voltage_per_analog_signal = 0.004882814;
  return (analogRead(pin) * voltage_per_analog_signal);
}

void loop_lcd() {
  // print the number of seconds since reset:
  int all_seconds = (wash_time - millis() + cleaning_start_time)/(1000);
  int minutes = all_seconds / 60;
  int seconds = all_seconds % 60;
  if(minutes >= 0 && seconds >= 0){
    lcd.setCursor(0, 1);
    print_two_digits(minutes);
    lcd.setCursor(2, 1);
    lcd.print(':');
    lcd.setCursor(3, 1);
    print_two_digits(seconds);
  }
}

boolean is_complete(){
  int all_seconds = (wash_time - millis() + cleaning_start_time)/(1000);
  int minutes = all_seconds / 60;
  int seconds = all_seconds % 60;
  if(int(minutes) == 0 && int(seconds) == 0){
    Serial.println("Done");
    return true;
  } else {
    return false;
  }
}

void notify_complete(){
  
}

void setup_lcd(){
  // set up the LCD's number of columns and rows: 
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Cleaning Now!");
}

void print_two_digits(int digits){
  if (digits < 10){
    lcd.print(0);
  }
  lcd.print(digits);
}

void reset_timer(){
  cleaning_start_time = millis();
}

