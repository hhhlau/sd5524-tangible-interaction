#include <Servo.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


String clientId = "sys1_1"; // --> Define your client ID is string, be aware NOT to have duplicated ID

// Update these with values suitable for your network.

const char* ssid = "H'wn";
const char* password = "20020535g";
const char* mqtt_server = "sd5524-2-broker.cloud.shiftr.io";

// ----------------------------------------------------------------------------------
//                      Custom Golabal Var.
// ----------------------------------------------------------------------------------

String topic_head("sd5524/2");  // --> Define your Topic prefix
String shared_pubTopic = topic_head + String("/status");
String shared_subTopic = topic_head + String("/") + clientId + String("/shared");
String self_subTopic = topic_head + String("/device/") + clientId;
// ----------------------------------------------------------------------------------


// ----------------------------------------------------------------------------------
//                      Custom Golabal Var.
// ----------------------------------------------------------------------------------
Servo servo;
int initAngle = 60;
Servo servoObjs[] = {servo};
int servoPins[] = {D8};
bool isServoSetUp = false;

const int motor1Pin1 = D0;
const int motor1Pin2 = D1;
const int motor1SpeedPin = D2;

int motor1Speed = 0;
// ----------------------------------------------------------------------------------


int IndexToPin[] = {D0, D1, D2, D3, D4, D5, D6, D7, D8, A0};

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


  String _myTopic = topic_head + String("/device/") + clientId;
  if ((String)topic == _myTopic) {
    servo.write(100);
    Serial.println("Hello");
  }
  if ((String)topic == _myTopic + String("/setup")) {
    int _pin = (int)doc["pin"];
    int _sAngle = (int)doc["startAngle"];
    Serial.println(_pin);
    Serial.println(_sAngle);
    setUpServo(_pin, _sAngle);
    String _msg("Set up pin: " + (String)_pin + " with init angle at " + (String)_sAngle);
    statusPublisher(_msg, (String)topic);

  }
  if ((String)topic == _myTopic + String("/turn")) {
    int _servoId = (int)doc["servoId"];
    int _servoAngle = (int)doc["angle"];
    Serial.println(_servoAngle);
    servoObjs[_servoId].write(_servoAngle);
    String _msg("Set the servo " + (String)_servoId + " angle to " + (String)_servoAngle) ;
    statusPublisher(_msg, (String)topic);
    delay(100);
  }

  if ((String)topic == _myTopic + String("/run")) {
    int _speed = (int)doc["speed"];
    int _isClockwise = (bool)doc["isClockwise"];
    motor1Speed = _speed;
    analogWrite(motor1SpeedPin, motor1Speed);
    //    servoObjs[_servoId].write(_servoAngle);
    setRollerDirection(_isClockwise);
    long _speedPert = (motor1Speed / 1023) * 100;
    String _msg("Setting motor 1  at " + (String)_speedPert + "% speed in clockwise.");
    statusPublisher(_msg, (String)topic);
  }

  if ((String)topic == _myTopic + String("/init")) {
    setUpServo(D8, 0);
    String _msg("Initializing Servo, setting up pin:D8 with init angle at 0 ");
    statusPublisher(_msg, (String)topic);
    delay(100);
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
    if (client.connect(clientId.c_str(),"sd5524-2-broker","jdNd7DyzTnSPhFtB")) {
      Serial.print("connected as: ");
      Serial.println(clientId);

      client.subscribe(self_subTopic.c_str());

      // Subcribed to wildcard topic '.../device/{clientId}/#'
      String wildCardChannel = topic_head + String("/device/") + clientId + String("/#");
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
void statusPublisher(String _msg, String _topic) {

  StaticJsonDocument<256> pubDoc;
  pubDoc["id"] = clientId;
  pubDoc["echoFrom"] = _topic;
  pubDoc["msg"] = _msg;
  pubDoc["test"] = "{'data':[1,2,3]}";
  Serial.println(_msg);
  Serial.println("------->!!");
  char buffer[256];
  size_t n = serializeJson(pubDoc, buffer);
  client.publish(shared_pubTopic.c_str(), buffer, n);
}

void setUpServo(int pin, int startAngle) {
  if (servo.attached()) {
    servo.detach();
    Serial.println("Detaching Servo with pervious setting...");
  }
  servoPins[0] = IndexToPin[pin];
  initAngle = startAngle;
  pinMode(servoPins[0], OUTPUT);
  servo.attach(servoPins[0]);
  servo.write(initAngle);

  isServoSetUp = true;
}

void setRollerDirection(bool isClockwise) {
  if (isClockwise) {
    digitalWrite(motor1Pin1, LOW);
    digitalWrite(motor1Pin2, HIGH);
  } else {
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

  pinMode(servoPins[0], OUTPUT);
  servoObjs[0].attach(servoPins[0]);
  servoObjs[0].write(initAngle);

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
