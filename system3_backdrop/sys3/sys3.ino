#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include <Adafruit_NeoPixel.h>

String clientId = "sys3_1"; // --> Define your client ID is string, be aware NOT to have duplicated ID

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
// ----------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------
//                      Custom Golabal Var.
// ----------------------------------------------------------------------------------
//1. Set up neopixel/ WS2812B
const int pixelPin = D6;
const int numOfPixels = 8;
int myR = 0;
int myG = 0;
int myB = 0;
Adafruit_NeoPixel pixels(numOfPixels, pixelPin, NEO_GRB + NEO_KHZ800);

const int motor1Pin1 = D0;
const int motor1Pin2 = D1;
const int motor1SpeedPin = D2;

int motor1Speed = 550;

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

//  MQTT topic handlers
  if((String)topic == _myTopic + String("/setup")){


  }
  if((String)topic == _myTopic + String("/turn")){
    int _speed = (int)doc["speed"];
    int _isClockwise = (bool)doc["isClockwise"];
    motor1Speed = _speed;
    analogWrite(motor1SpeedPin, motor1Speed);
//    servoObjs[_servoId].write(_servoAngle);
    setRollerDirection(_isClockwise);
    long _speedPert = (motor1Speed/ 1023)* 100;
    String _msg("Setting motor 1  at "+(String)_speedPert +"% speed in clockwise.");
    statusPublisher(_msg, (String)topic);
  }
  
  if((String)topic == _myTopic + String("/pixel/init")){
    int _numOfPixels = (int)doc["numOfPixels"];
    if (_numOfPixels){
      pixels.clear();
      pixels.updateLength(_numOfPixels);
      String _msg("The pixels number is set to "+(String)_numOfPixels);
      statusPublisher(_msg, (String)topic);
    }else{
      String _msg("Incorrect para, init suspended");
      statusPublisher(_msg, (String)topic);
    }
  }

  if((String)topic == _myTopic + String("/pixel/set")){
    int _r = (int)doc["red"];
    int _g = (int)doc["green"];
    int _b = (int)doc["blue"];
    int _brightness = (int)doc["brightness"];
    
    if (_brightness){
      setPixelColor(_r, _g, _b);
      int mappedBrightness = map(_brightness, 0, 100, 0, 255);
      pixels.setBrightness(mappedBrightness);
      String _msg("Setting pixel strip with R: "+(String)_r +", G:"+(String)_g +", B:"+(String)_b+" with brightness: "+ (String)_brightness + "%");
      statusPublisher(_msg, (String)topic);
    }else {
      setPixelColor(_r, _g, _b);
      String _msg("Setting pixel strip with R: "+(String)_r +", G:"+(String)_g +", B:"+(String)_b);
    statusPublisher(_msg, (String)topic);
    }
    
    
    
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

void setPixelColor(int r, int g, int b){
  myR = r;
  myG = g;
  myB = b;
  for(int i=0; i<numOfPixels; i++) {
    pixels.setPixelColor(i, pixels.Color(myR, myG, myB));
  }
  pixels.show();   // Send the updated pixel colors to the hardware.
  
}

void setRollerDirection(bool isClockwise) {
  if (isClockwise){
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
  }else {
    digitalWrite(motor1Pin1, HIGH);
    digitalWrite(motor1Pin2, LOW);
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


//  Custom set up
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();
  
  pinMode(motor1Pin1, OUTPUT);
  pinMode(motor1Pin2, OUTPUT);
  pinMode(motor1SpeedPin, OUTPUT);

  setRollerDirection(true);

  analogWrite(motor1SpeedPin, motor1Speed);


}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

}
