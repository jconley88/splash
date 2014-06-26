int flexSensorPin = A0; //analog pin 0

const int keep = 20;
const int threshold = 200;
int values[keep];
int valcounter = 0;

void setup(){
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
  
  Serial.print("Sensor Reading: ");
  Serial.print(flexSensorReading);
  Serial.print(" running: ");
  Serial.println(running);

  //In my tests I was getting a reading on the arduino between 512, and 614. 
  //Using map(), you can convert that to a larger range like 0-100.
  int flex0to100 = map(flexSensorReading, 512, 614, 0, 100);
  //Serial.println(flex0to100);

  delay(250); //just here to slow down the output for easier reading
}
