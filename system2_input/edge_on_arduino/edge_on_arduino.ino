// ----------------------------------------------------------------------------------
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int analogInPin2 = A1;
const int ledPin = 2;

int sensorValue = 0;        // value read from the pot
int oldValue = 0;

int sensorValue2 = 0;        // value read from the pot
int oldValue2 = 0;

bool isDebug = true;

bool isCalibrated = false;
int calibrationCounter = 0;
int calibrationRef = 0;

long standingCounter = 0;
bool isStanding = false;
bool isBoardcastedStanding = false;

int sensitivity = 20;
int standingBounce = 10;
int refValue = 1024; // Largest value that A0 will output

long lastStamp = 0;


const int outSignalPin = 8;
const int outStandingPin = 7;
const int inSettingPin = 6;


// ----------------------------------------------------------------------------------

// ----------------------------------------------------------------------------------
//                                Custom Functions here  
// ----------------------------------------------------------------------------------
int deltaChange(int newVal){
  return newVal - sensorValue;
}

long deltaTimeStamp() {
  long _current = millis();
  long _last = lastStamp;
  lastStamp = _current;
  return _current - _last;
}

void checkIfStanding(int delta) {
  if (abs(delta) < standingBounce && sensorValue != refValue){
    long _counter = standingCounter;
    standingCounter = millis();
    if(standingCounter - _counter > 5000){
      Serial.println("User is standing.");
      isStanding = true;
    }
  }else {
    isStanding = false;
    isBoardcastedStanding = false;
    standingCounter = 0;
  }
}

int pressureDetection(int pin) {
  int readVal = analogRead(pin); 
  int deltaVal =deltaChange(readVal); 

   if (isDebug){
      Serial.print("----------------");
      Serial.print((String)pin);
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

void calibrate(int pin) {
  if (calibrationCounter < 5){
    int tempVal = pressureDetection(pin);
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

void signalEmitLogic(int oV, int nV){
  if ( oV-nV > sensitivity ){
    signalBlinker(outSignalPin);
//            long _delta = deltaTimeStamp();
//            Serial.println(_delta);
//            emitSignal(_delta);
            
      }
}

void signalBlinker(int pin){
  digitalWrite(pin, HIGH);
  delay(100);
  digitalWrite(pin, LOW);
}

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(analogInPin, INPUT);
  pinMode(analogInPin2, INPUT);

  pinMode(outSignalPin, OUTPUT);
  pinMode(outStandingPin, OUTPUT);
  pinMode(inSettingPin, INPUT);

  sensorValue = pressureDetection(analogInPin);
  sensorValue2 = pressureDetection(analogInPin2);

}

void loop() {
  oldValue = sensorValue;
    sensorValue = pressureDetection(analogInPin);

    oldValue2 = sensorValue2;
    sensorValue2 = pressureDetection(analogInPin2);
//   put your main code here, to run repeatedly:
  if (isCalibrated){
    oldValue = sensorValue;
    sensorValue = pressureDetection(analogInPin);

    oldValue2 = sensorValue2;
//    sensorValue2 = pressureDetection(analogInPin2);

//    Logic here
    checkIfStanding(oldValue-sensorValue);
    if (isStanding){
      if (!isBoardcastedStanding){
//        emitStanding();
        isBoardcastedStanding = true;
      }
    }else {
      signalEmitLogic(oldValue, sensorValue);
//      signalEmitLogic(oldValue2, sensorValue2);
    }
    
  }else {
    sensorValue = pressureDetection(analogInPin);
    calibrate(analogInPin);
    sensorValue2 = pressureDetection(analogInPin2);
    calibrate(analogInPin2);
  }

  signalBlinker(outSignalPin);
  
  delay(2000) ;        

}
