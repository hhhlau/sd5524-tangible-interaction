#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


String clientId = "sys2_1"; // --> Define your client ID is string, be aware NOT to have duplicated ID

// Update these with values suitable for your network.

const char* ssid = "H'wn";
const char* password = "20020535g";
const char* mqtt_server = "158.132.54.138";

// ----------------------------------------------------------------------------------
//                      Custom Golabal Var.
// ----------------------------------------------------------------------------------

String topic_head("sd5524/2");  // --> Define your Topic prefix
String shared_pubTopic=topic_head+String("/status");
String shared_subTopic=topic_head+String("/")+clientId+String("/shared");
String self_subTopic = topic_head + String("/device/")+clientId;
String self_pubTopic = topic_head + String("/device/")+clientId + String("/status");
// ----------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------
//                      Custom Golabal Var.
// ----------------------------------------------------------------------------------
const int analogInPin = A0;  // Analog input pin that the potentiometer is attached to
const int ledPin = 2;

int sensorValue = 0;        // value read from the pot
int oldValue = 0;

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
// ----------------------------------------------------------------------------------


int IndexToPin[]={D0,D1,D2,D3,D4,D5,D6,D7,D8,A0};

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println(topic);

//  Handle JSON payload
  StaticJsonDocument<256> doc;
    // Deserialize the JSON document
  DeserializationError error = deserializeJson(doc, payload, length);

  // Test if parsing succeeds.
  if (error) {
    Serial.print(F("deserializeJson() failed: "));
    Serial.println(error.f_str());
    return;
  }

  
  String _myTopic = topic_head + String("/device/")+clientId;
   if((String)topic == _myTopic){
     Serial.println("Hello");
   }
  if((String)topic == _myTopic + String("/setup")){
    int _sensitivity = (int)doc["sensitivity"];
    String _msg("Setting from sensitivity" + (String)sensitivity+ " -> " + (String)_sensitivity);
    sensitivity = _sensitivity;
    statusPublisher(_msg, (String)topic);

  }

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
//    String clientId = "ESP8266Client-";
//    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.print("connected as: ");
      Serial.println(clientId);

      client.subscribe(self_subTopic.c_str());

   // Subcribed to wildcard topic '.../device/{clientId}/#'
      String wildCardChannel = topic_head + String("/device/")+clientId+ String("/#");
      client.subscribe(wildCardChannel.c_str());

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

// ----------------------------------------------------------------------------------
//                                Custom Functions here  
// ----------------------------------------------------------------------------------
void statusPublisher(String _msg, String _topic){
  
  StaticJsonDocument<256> pubDoc;
    pubDoc["id"] = clientId;
    pubDoc["echoFrom"] = _topic;
    pubDoc["msg"]= _msg;
    Serial.println(_msg);
    char buffer[256];
    size_t n = serializeJson(pubDoc, buffer);
    client.publish(shared_pubTopic.c_str(), buffer, n);
}

void emitSignal(long _delta){
  StaticJsonDocument<256> pubDoc;
    pubDoc["id"] = clientId;
    pubDoc["msg"]= "pressed";
    pubDoc["delta"] = _delta;
    char buffer[256];
    size_t n = serializeJson(pubDoc, buffer);
    client.publish(self_pubTopic.c_str(), buffer, n);
}

void emitStanding(){
  StaticJsonDocument<256> pubDoc;
    pubDoc["id"] = clientId;
    pubDoc["msg"]= "stopped";
    pubDoc["standingCounter"] = standingCounter;
    char buffer[256];
    size_t n = serializeJson(pubDoc, buffer);
    client.publish(self_pubTopic.c_str(), buffer, n);
}


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

// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

void setup() {
//  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
  
  pinMode(analogInPin, INPUT);
  sensorValue = pressureDetection();
    
  

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  if (isCalibrated){
    oldValue = sensorValue;
    sensorValue = pressureDetection();

//    Logic here
    checkIfStanding(oldValue-sensorValue);
    if (isStanding){
      if (!isBoardcastedStanding){
        emitStanding();
        isBoardcastedStanding = true;
      }
    }else {
      if ( oldValue-sensorValue > sensitivity ){
            long _delta = deltaTimeStamp();
            Serial.println(_delta);
            emitSignal(_delta);
            
      }else{
        digitalWrite(2, LOW);
      }
    }
    
  }else {
    sensorValue = pressureDetection();
    calibrate();
  }
  
  delay(500) ;        

}
