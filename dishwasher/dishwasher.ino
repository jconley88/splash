#include <LiquidCrystal.h>
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int flexSensorPin = A0; //analog pin 0

const int keep = 20;
const int threshold = 200;
int values[keep];
int valcounter = 0;
const int temperaturePin = 1;

void setup(){
  setup_lcd();
  Serial.begin(9600);
  for(int i = 0; i < keep; ++i)
    values[i] = 0;
}

void loop(){
  int flexSensorReading = analogRead(flexSensorPin); 

  //Serial.println(flexSensorReading);
  values[++valcounter % keep] = flexSensorReading;

  int minval = 1024, maxval = 0;
  for(int i = 0; i < keep; ++i) {
    minval = min(minval, values[i]);
    maxval = max(maxval, values[i]);
  }
  
  Serial.print("range: ");
  Serial.print(maxval-minval);
  Serial.print(" max: ");
  Serial.print(maxval);
  Serial.print(" min: ");
  Serial.println(minval);
  
    
  int running = 0;
  if(maxval - minval > 200) {
    running = 1;
  }

  lcd.setCursor(0, 0);
  if(running == 1) {
    lcd.print("Cleaning Now!");
  } else {
    lcd.print("Not Cleaning!");
  }

  Serial.print("Sensor Reading: ");
  Serial.print(flexSensorReading);
  Serial.print(" running: ");
  Serial.println(running);

  loop_lcd();
  loop_temperature();
  delay(250); //just here to slow down the output for easier reading
}

void loop_temperature(){
  float voltage, degreesF, degreesC;
  voltage = getVoltage(temperaturePin);
  degreesC = (voltage - 0.5) * 100.0;
  degreesF = degreesC * (9.0/5.0) + 32.0;

  lcd.setCursor(15, 1);
  lcd.print('F')
  int lcd_position = 15
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
  int all_seconds = (2715000 - millis())/(1000);
  int minutes = all_seconds / 60;
  int seconds = all_seconds % 60;
  
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  print_two_digits(minutes);
  lcd.setCursor(2, 1);
  lcd.print(':');
  lcd.setCursor(3, 1);
  print_two_digits(seconds);
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
