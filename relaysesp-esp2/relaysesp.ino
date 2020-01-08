#include <ESP8266WiFi.h>

#include <PubSubClient.h>

// Update these with values suitable for your network.
const char* ssid = "popcorn1234";
const char* password = "internetofp";
const char* mqtt_server = "192.168.4.1";
const char *MQTT_USER = "admin"; // NULL for no authentication
const char *MQTT_PASSWORD = "admin"; // NULL for no authentication
#define MQTT_TOPIC_DOORSTATE "ha/door"
#define MQTT_TOPIC_VESSELSTATE "ha/vessel"
#define MQTT_TOPIC_STATE "ha/status"
#define MQTT_PUBLISH_DELAY 200

WiFiClient espClient;
PubSubClient client(espClient);
int relay1 = 14;
int relay2 = 12;
int door = D1;
int ypin = A0;
String switch1;
String switch2;
String strTopic;
String strPayload;
long lastMsgTime = 0;
int doorstate;
int vesselstate;
void setup_wifi() {

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  payload[length] = '\0';
  strTopic = String((char*)topic);
  if(strTopic == "ha/relay1")
    {
    switch1 = String((char*)payload);
    if(switch1 == "ON")
      {
        Serial.println("ON");
        digitalWrite(relay1, HIGH);
      }
    else
      {
        Serial.println("OFF");
        digitalWrite(relay1, LOW);
      }
    }
    
     if(strTopic == "ha/relay2")
    {
    switch2 = String((char*)payload);
    if(switch2 == "ON")
      {
        Serial.println("ON");
        digitalWrite(relay2, HIGH);
      }
    else
      {
        Serial.println("OFF");
        digitalWrite(relay2, LOW);
      }
    }
}
 
 
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("arduinoClient",MQTT_USER,MQTT_PASSWORD)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.subscribe("ha/#");
      client.publish(MQTT_TOPIC_STATE, "connected", true);
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
 
void setup()
{
  Serial.begin(115200);
  setup_wifi(); 
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  pinMode(relay1, OUTPUT);
  digitalWrite(relay1, HIGH);
 pinMode(relay2, OUTPUT);
  digitalWrite(relay2, HIGH);
  //pinMode(door, INPUT);
  pinMode(door, INPUT_PULLUP);
  pinMode(vesselstate, INPUT);
}
 
void loop()
{
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
 // delay(5000);
 long now = millis();
 if (now - lastMsgTime > MQTT_PUBLISH_DELAY) {
    lastMsgTime = now;
    doorstate=digitalRead(door);
    vesselstate=analogRead(ypin);
  mqttPublish(MQTT_TOPIC_DOORSTATE, doorstate);
  mqttPublish(MQTT_TOPIC_VESSELSTATE, vesselstate);
  }
}

void mqttPublish(char* topic, float payload) {
Serial.print(topic);
  Serial.print(": ");
 Serial.println(payload);
  client.publish(topic, String(payload).c_str(), true);
}
