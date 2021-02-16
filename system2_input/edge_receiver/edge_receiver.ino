#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

const int inSignalPin = D8;
const int inStandingPin = D7;
const int outSettingPin = D6;
int signalReading = 0;

long initTimeStamp = 0;
long lastTimeStamp = 0;

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
//    String _msg("Setting from sensitivity" + (String)sensitivity+ " -> " + (String)_sensitivity);
//    sensitivity = _sensitivity;
//    statusPublisher(_msg, (String)topic);

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
    pubDoc["sinceSys"] = millis();
    char buffer[256];
    size_t n = serializeJson(pubDoc, buffer);
    client.publish(self_pubTopic.c_str(), buffer, n);
}

void emitStanding(){
//  StaticJsonDocument<256> pubDoc;
//    pubDoc["id"] = clientId;
//    pubDoc["msg"]= "stopped";
//    pubDoc["standingCounter"] = standingCounter;
//    char buffer[256];
//    size_t n = serializeJson(pubDoc, buffer);
//    client.publish(self_pubTopic.c_str(), buffer, n);
}

void signalDetected() {
  long _currentTimeStamp = millis();
  long _deltaStamp = deltaTimeStamp(_currentTimeStamp);
  _deltaStamp = _deltaStamp/ 1000;

  emitSignal(_deltaStamp);
  
  Serial.println();
  Serial.println("*******************************");
  Serial.println("Signal Detected:");
  Serial.println("Signal time difference: "+ (String)_deltaStamp +"s");
  Serial.println("Last: "+ (String)lastTimeStamp+ " , Current: "+ (String)_currentTimeStamp);
  Serial.println("*******************************");
  lastTimeStamp = _currentTimeStamp;

}

long deltaTimeStamp(long newStamp){
  Serial.println("~~~~");
  Serial.println(newStamp - lastTimeStamp);
  return newStamp - lastTimeStamp ;
}



// ----------------------------------------------------------------------------------
// ----------------------------------------------------------------------------------

void setup() {
//  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(inSignalPin, INPUT);
  pinMode(inStandingPin, INPUT);
  pinMode(outSettingPin, OUTPUT);
  
//  pinMode(analogInPin, INPUT);
  initTimeStamp = millis();

  StaticJsonDocument<256> pubDoc;
    pubDoc["id"] = clientId;
    pubDoc["msg"]= "init";
    pubDoc["initTimeStamp"] = initTimeStamp;
    char buffer[256];
    size_t n = serializeJson(pubDoc, buffer);
    client.publish(self_pubTopic.c_str(), buffer, n);
    
  

}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  signalReading = digitalRead(inSignalPin);

  if (signalReading) {
    signalDetected();
  }
  delay(1000);

     

}
