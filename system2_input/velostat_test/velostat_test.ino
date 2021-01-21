const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int ledPin = 2;

int sensorValue = 0;        // value read from the pot
int oldValue = 0;

bool isDebug = false;

bool isCalibrated = false;
int calibrationCounter = 0;
int calibrationRef = 0;

int sensitivity = 20;
int refValue = 1023;
 
void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(9600);
  pinMode(analogInPin, INPUT);
  pinMode(7, INPUT);
 pinMode(ledPin, OUTPUT);

 sensorValue = pressureDetection();

 
}
 
void loop() {
  // read the analog in value:
  if (isCalibrated){
    oldValue = sensorValue;
    sensorValue = pressureDetection();

//    Logic here
    if ( oldValue-sensorValue > sensitivity ){
          digitalWrite(2, HIGH);
    }else{
      digitalWrite(2, LOW);
    }
    
  }else {
    sensorValue = pressureDetection();
    calibrate();
  }
  
  delay(500) ;           
}

int deltaChange(int newVal){
  return newVal - sensorValue;
}

int pressureDetection() {
  int readVal = analogRead(analogInPin); 
  int deltaVal =deltaChange(readVal); 

   if (isDebug){
      Serial.println("----------------");
      Serial.print("past Val = " );                       
      Serial.println(sensorValue); 
      Serial.print("sensor = " );                       
      Serial.print(readVal); 
      if (readVal != sensorValue) {
        Serial.print("(");
        Serial.print(deltaVal); 
        Serial.println(")");
      }else {
        Serial.println();
      }
      Serial.println("----------------");
      Serial.println();
   }

  return readVal;
}

void calibrate() {
  if (calibrationCounter < 5){
    int tempVal = pressureDetection();
    if (sensorValue - tempVal < 5){
      calibrationCounter++;
    }else {
      calibrationCounter = 0;
    }
    sensorValue = tempVal;
  }else if (calibrationCounter == 5){
    calibrationRef = sensorValue;
    Serial.println("The Velostat is calibrated.");
    Serial.print("The base value is : ");
    Serial.println(calibrationRef);
    isCalibrated = true;
  }
  
}
